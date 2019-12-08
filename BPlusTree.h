#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <vector>
#include <ostream>
#include <cassert>
#include <stack>
#include <algorithm>

using namespace std;

template<typename T>
struct Node {
    enum NodeType {
        KEY,
        DATA
    };

    explicit Node(NodeType type = DATA) : type{type} {}
    const vector<Node*>& getPointers() const { return ptrs; }
    NodeType getType() const {return type; }
    NodeType type;       // type of node
    vector<Node *> ptrs; // pointers (KEY: pointers to other nodes, DATA: pointer to next data node)
    vector<T> vals;      // values (KEY: keys, DATA: data)
};

template<typename T>
class BPlusTree {
    template<typename R>
    friend class BPlusTreeRenderer;
private:
    void insertRecursive(Node<T>* node, const T& val);  // insert into tree
    void fixChild(Node<T>* parent, Node<T>* child);

    size_t M;    // order of table
    size_t L;    // amount of data in data nodes
    Node<T> * root;  // root
    size_t size; // current amount of data stored

public:
// Constructors
    // default constructor: L=M-1
    explicit BPlusTree(size_t M = 5);
    // other constructor
    BPlusTree(size_t M, size_t L);
    // destructor
    ~BPlusTree();

// Big N
    // copy constructor
    BPlusTree(const BPlusTree & other);
    // move constructor
    BPlusTree(BPlusTree && other);
    // copy assignment operator
    BPlusTree & operator=(const BPlusTree & other);
    // move assignment operator
    BPlusTree & operator=(BPlusTree && other);


// Modifiers
    void insert(const T & val);  // insert into tree
    void remove(const T & val);  // remove from tree
    void makeEmpty();            // make the tree empty
    void setMAndClear(int m) {
        makeEmpty();
        this->M = m;
        this->L = m - 1;
    }
    void setLAndClear(int l) {
        makeEmpty();
        this->L = l;
    }

// Getters
    // get M
    size_t getM() const;
    size_t getL() const;
    // does the tree contain val?
    bool contains(const T & val) const;
    
    Node<T>* findPos(const T &val) const;
    // is the tree empty? (root==nullptr)
    bool isEmpty() const;

    template <class Key>
    // print data in order of least to greatest
    friend ostream & operator<<(ostream & out, const BPlusTree<Key>& tree);

    static BPlusTree<T> makeTestTree();
};

// All functions are implemented in the .h file because C++ is jank with
// template classes

const bool NOT_IMPLEMENTED = false;

template<typename T>
BPlusTree<T>::BPlusTree(size_t M)
        : M{M}, root{nullptr}, size{0}, L{M-1} {}

template<typename T>
BPlusTree<T>::BPlusTree(size_t M, size_t L)
        : M{M}, L{L}, root{nullptr}, size{0} {}

template<typename T>
BPlusTree<T>::~BPlusTree() {
    if (!root)
        return;
    stack<Node<T>*> toDelete;
    toDelete.push(root);
    while (!toDelete.empty()) {
        Node<T>* node = toDelete.top();
        toDelete.pop();
        if (node->getType() == Node<T>::KEY) {
            for (Node<T>* n : node->getPointers()) {
                toDelete.push(n);
            }
        }
        delete node;
    }
}

template<typename T>
BPlusTree<T>::BPlusTree(const BPlusTree &other) {
    M = other.M;
    L = other.L;
    root = nullptr;
    size = other.size;
    //if the other tree contains data, copy it.
    if(!other.isEmpty()) {
        //establish a pointer to the other root
        Node<T> *adder = other.root;
        //move the pointer down to the leaf (aka DATA) level for inserts
        while (adder->type != Node<T>::DATA)
            adder = adder->ptrs[0];
        while(adder){
            //add all the values in the current data node
            for(auto k = adder->vals.begin(); k != adder->vals.end(); k++)
                insert(*k);
            //move to next data node. If it's null, we're done.
            adder = adder->ptrs[0];
        }
    }

}

template<typename T>
BPlusTree<T>::BPlusTree(BPlusTree &&other) {
    M = other.M;
    L = other.L;
    root = other.root;
    size = other.size;
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree &other) {
    //only carry out if the other tree isn't this one
    if(this->root != other.root){
        //clear our tree
        if(!this->isEmpty())
            this->makeEmpty();
        M = other.M;
        L = other.L;
        size = other.size;
        //if the other tree contains data, copy it.
        if(!other.isEmpty()) {
            //establish a pointer to the other root
            Node<T> *adder = other.root;
            //move the pointer down to the leaf (aka DATA) level for inserts
            while (adder->type != Node<T>::DATA)
                adder = adder->ptrs[0];
            while(adder){
                //add all the values in the current data node
                for(auto k = adder->vals.begin(); k != adder->vals.end(); k++)
                    insert(*k);
                //move to next data node. If it's null, we're done.
                adder = adder->ptrs[0];
            }
        }
    }

    return *this;
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator=(BPlusTree &&other) {
    //only carry out if the other tree isn't this one
    if(this->root != other.root){
        if(!this->isEmpty())
            this->makeEmpty();
        M = other.M;
        L = other.L;
        root = other.root;
        size = other.size;
    }
}

template<typename T>
void BPlusTree<T>::insertRecursive(Node<T> *node, const T &val) {
    if (node->getType() == Node<T>::KEY) {
        int i = 0;
        while (i < node->vals.size() && val >= node->vals[i])
            ++i;
        Node<T>* child = node->ptrs[i];
        // Insert value to child
        insertRecursive(node->ptrs[i], val);
        fixChild(node, child);
    }
    else {
        auto placeToInsert = upper_bound(node->vals.begin(), node->vals.end(), val);
        node->vals.insert(placeToInsert, val);
    }
}

template<typename T>
void BPlusTree<T>::fixChild(Node<T>* parent, Node<T>* child) {

    if ((child->type == Node<T>::DATA && child->vals.size() > L)
        || (child->type == Node<T>::KEY && child->ptrs.size() > M))
    {
        if (child == root) {
            parent = new Node<T>(Node<T>::KEY);
            parent->ptrs.push_back(child);
            this->root = parent;
        }

        Node<T>* newNode = new Node<T>(child->getType());

        // move vals (median to end) to new node
        int medianIndex = child->vals.size()/ 2;
        if (child->type == Node<T>::DATA) {
            for (int i = medianIndex; i < child->vals.size(); ++i)
                newNode->vals.push_back(child->vals[i]);
            parent->ptrs.push_back(newNode);
            parent->vals.push_back(newNode->vals[0]);
            child->vals.resize(medianIndex);
            Node<T>* oldNextData = child->ptrs[0];
            child->ptrs[0] = newNode;
            newNode->ptrs.push_back(oldNextData);
        }
        else {
            // Don't copy the median to the new node, only to parent
            for (int i = medianIndex + 1; i < child->vals.size(); ++i) {
                newNode->vals.push_back(child->vals[i]);
            }
            for (int i = medianIndex + 1; i < child->ptrs.size(); ++i) {
                newNode->ptrs.push_back(child->ptrs[i]);
            }

            parent->ptrs.push_back(newNode);
            parent->vals.push_back(child->vals[medianIndex]);
            child->vals.resize(medianIndex);
            child->ptrs.resize(medianIndex + 1);
        }
    }
}

template<typename T>
void BPlusTree<T>::insert(const T &val) {
    if (root == nullptr){
        //if tree does not exist create a new data node with val
        root = new Node<T>;
        root->vals.push_back(val);
        root->ptrs.push_back(nullptr);
    }
    else {
        insertRecursive(root, val);
        fixChild(nullptr, root);
    }
    size++;
}

template<typename T>
void BPlusTree<T>::remove(const T &val) {
    assert(NOT_IMPLEMENTED);
    Node<T>* current = root;
    
    while (current->getType() == Node<T>::KEY){
        for (int i = 0; i < current->vals.size(); i++) {
            current = current->ptrs[current->ptrs.size()];
            if (val < current->vals[i]){
                current = current->ptrs[i];
                break;
            }
        }
    }
    for (int i = 0; i < current->vals.size(); i++) {
        if (val == current->vals[i]){
            current->vals.insert(current->vals.begin() + i, val);
        }
    }
}

template<typename T>
void BPlusTree<T>::makeEmpty() {
    if (root != nullptr){
        stack<Node<T>*> toDelete;
        toDelete.push(root);
        while (!toDelete.empty()) {
            Node<T>* node = toDelete.top();
            toDelete.pop();
            if (node->getType() == Node<T>::KEY) {
                for (Node<T>* n : node->getPointers()) {
                    toDelete.push(n);
                }
            }
            delete node;
        }
        root = nullptr;
        size = 0;
    }
}

template<typename T>
size_t BPlusTree<T>::getM() const {
    return M;
}

template<typename T>
size_t BPlusTree<T>::getL() const {
    return L;
}

template<typename T>
Node<T>* BPlusTree<T>::findPos(const T &val) const {
    Node<T>* current = root;
    while (current->getType() == Node<T>::KEY){
        for (int i = 0; i < current->vals.size(); i++) {
            current = current->ptrs[current->ptrs.size()];
            if (val < current->vals[i]){
                current = current->ptrs[i];
                break;
            }
        }
    }
    return current;
}

template<typename T>
bool BPlusTree<T>::contains(const T &val) const {
    Node<T>* current = root;
    bool found = false;
    while (current->getType() == Node<T>::KEY){
        current = current->ptrs.back();
        for (int i = 0; i < current->vals.size(); i++) {
            if (val < current->vals[i]){
                current = current->ptrs[i];
                break;
            }
        }
    }
    for (int i = 0; i < current->vals.size() && !found; i++) {
        if (val == current->vals[i]){
            current->vals.insert(current->vals.begin() + i, val);
            found = true;
        }
    }
    return found;
}

template<typename T>
bool BPlusTree<T>::isEmpty() const {
    return root == nullptr;
}

template<class Key>
ostream &operator<<(ostream &out, const BPlusTree<Key> &tree) {
    Node<Key>* ptr = tree.root;
    while (ptr->getType() == Node<Key>::KEY){
        ptr = ptr->ptrs.front();
    }
    int index;
    for (int i = 0; i < tree.size(); ++i){
        index = 0;
        cout << ptr->vals[index] << " ";
        if (index >= ptr->vals.size()){
            ptr = ptr->ptrs[0];
            index = 0;
        }
        index++;
    }
}

template <class T>
BPlusTree<T> BPlusTree<T>::makeTestTree() {
    auto bottom8 = new Node<T>(Node<T>::DATA);
    bottom8->vals = {987, 1597, 9999};
    bottom8->ptrs = {nullptr};
    auto bottom7 = new Node<T>(Node<T>::DATA);
    bottom7->vals = {377, 610};
    bottom7->ptrs = {bottom8};
    auto bottom6 = new Node<T>(Node<T>::DATA);
    bottom6->vals = {144, 233};
    bottom6->ptrs = {bottom7};
    auto bottom5 = new Node<T>(Node<T>::DATA);
    bottom5->vals = {55, 89};
    bottom5->ptrs = {bottom6};
    auto bottom4 = new Node<T>(Node<T>::DATA);
    bottom4->vals = {21, 34};
    bottom4->ptrs = {bottom5};
    auto bottom3 = new Node<T>(Node<T>::DATA);
    bottom3->vals = {8, 13};
    bottom3->ptrs = {bottom4};
    auto bottom2 = new Node<T>(Node<T>::DATA);
    bottom2->vals = {3, 5};
    bottom2->ptrs = {bottom3};
    auto bottom1 = new Node<T>(Node<T>::DATA);
    bottom1->vals = {1, 2};
    bottom1->ptrs = {bottom2};

    auto midLeft = new Node<T>(Node<T>::KEY);
    midLeft->vals = {3, 8};
    midLeft->ptrs = {bottom1, bottom2, bottom3};
    auto midRight = new Node<T>(Node<T>::KEY);
    midRight->vals = {55, 144, 377, 987};
    midRight->ptrs = {bottom4, bottom5, bottom6, bottom7, bottom8};

    auto top = new Node<T>(Node<T>::KEY);
    top->vals = {21};
    top->ptrs = {midLeft, midRight};

    BPlusTree tree;
    tree.M = 5;
    tree.L = 4;
    tree.root = top;
    tree.size = 17;
    return tree;
}

#endif // BPLUSTREE_H
