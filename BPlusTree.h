#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <vector>
#include <ostream>
#include <cassert>
#include <stack>

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
    size_t M;    // order of table
    size_t L;    // amount of data in data nodes
    Node<T> * root;  // root
    //TODO: Is the the amount of data stored at the bottom in the data nodes,
    // or for all nodes?
    // ^ Amount of Data ^
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
void BPlusTree<T>::insert(const T &val) {
    bool insert = false;
    // check if there is an exisiting tree of not
    if (root == nullptr){
        //if tree does not extist create a new data node with val
        root = new Node<T>;
        root->vals.push_back(val);
    } else {
        // if a tree does exist
        Node<T>* current = root;
        vector<Node<T>*> prev;
        prev.push_back(current);
        while (current->getType() == Node<T>::KEY){

            for (int i = 0; i < current->vals.size(); i++) {
                if (val < current->vals[i]){
                    current = current->ptrs[i];
                    break;
                }
            }
            if (current == prev.back()){
                current = current->ptrs[current->vals.size()];
            }
            prev.push_back(current);
        }
        for (int i = 0; i < current->vals.size() && !insert; i++) {
            if (val < current->vals[i]){
                current->vals.insert(current->vals.begin() + i, val);
                insert = true;
            }
        }
        if (!insert){
            current->vals.push_back(val);
        }
        // this part is buggy... very buggy...
        bool fixed = false;
        while(!prev.empty()){

            if (prev.back()->ptrs.size() > M){
                size_t index = M / 2;
                // get the value of the num pushed to a key
                T num = prev.back()->vals[index];
                // save the initial value of pointer and of the last visited
                Node<T>* initBack = prev.back();
                // if the node that need to be fixed is the root create a
                // new root KEY node that points to current data and add it to previous
                if (root == prev.back()){
                    root = new Node<T>(Node<T>::KEY);
                    root->ptrs.push_back(prev.back());
                    prev.insert(prev.begin(), root);
                }
                // to work with the node before the last node we touched
                prev.pop_back();
                // find the position to insert
                for (int i = 0; i < prev.back()->vals.size(); i++) {
                    // if the value at index is greater than num then the index is found
                    if (prev.back()->vals[i] >= num){
                        // move the num into the previous's values at index
                        prev.back()->vals.insert(prev.back()->vals.begin() + i, num);
                        // make a new KEY node in previous' pointrs at index
                        prev.back()->ptrs.insert(prev.back()->ptrs.begin() + (i + 1), new Node<T>(Node<T>::KEY));

                        // move the value at index and beyond to the new node's values
                        for (size_t j = index + 1; j < initBack->vals.size(); j++) {
                            prev.back()->ptrs[i+1]->vals.push_back(initBack->vals[j]);
                        }
                        // pop the values from initBack for vals.size-index
                        for (size_t j = index; j <= initBack->vals.size(); j++) {
                            initBack->vals.pop_back();
                        }
                        // move the pointers at index and beyond to the new node's pointers
                        for ( size_t j = index + 1; j < initBack->ptrs.size(); j++){
                            prev.back()->ptrs[i+1]->ptrs.push_back(initBack->ptrs[j]);
                        }
                        // pop the pointers from initial back for ptrs.size-index
                        for (size_t j = index; j <= initBack->ptrs.size(); j++) {
                            initBack->ptrs.pop_back();
                        }
                        // a flag to show that the node has been fixed and values are set
                        fixed = true;
                        break;
                    }
                }
                if (!fixed){
                    // move the num into the previous's values at index
                    prev.back()->vals.insert(prev.back()->vals.end(), num);

                    prev.back()->ptrs.insert(prev.back()->ptrs.end(), new Node<T>(Node<T>::KEY));

                    // move the value at index and beyond to the new node's values
                    for (size_t j = index + 1; j < initBack->vals.size(); j++) {
                        prev.back()->ptrs.back()->vals.push_back(initBack->vals[j]);
                    }
                    // pop the values from initBack for vals.size-index
                    for (size_t j = index; j <= initBack->vals.size(); j++) {
                        initBack->vals.pop_back();
                    }
                    // move the pointers at index and beyond to the new node's pointers
                    for ( size_t j = index + 1; j < initBack->ptrs.size(); j++){
                        prev.back()->ptrs.back()->ptrs.push_back(initBack->ptrs[j]);
                    }
                    // pop the pointers from initial back for ptrs.size-index
                    for (size_t j = index; j <= initBack->ptrs.size(); j++) {
                        initBack->ptrs.pop_back();
                    }
                }
            } else if (prev.back()->vals.size() > L){
                size_t index = L / 2 + 1;
                // get the value of the num pushed to a key
                T num = current->vals[index];
                // if the node that need to be fixed is the root create a
                // new root KEY node that points to current data and add it to previous
                if (root == prev.back()){
                    root = new Node<T>(Node<T>::KEY);
                    root->ptrs.push_back(prev.back());
                    prev.insert(prev.begin(), root);
                }
                // to work with the node before the last node we touched
                prev.pop_back();
                // find the position to insert
                for (int i = 0; i < prev.back()->vals.size(); i++) {
                    // if the previous node data is greater than number we found index
                    if (prev.back()->vals[i] >= num){
                        // move the num into the previous node's values at index
                        prev.back()->vals.insert(prev.back()->vals.begin() + i, num);
                        // create a new pointer to a node into the previous node's pointers at index
                        // the index is index + 1, ptrs has 1 when created and pointer right is greater than equal to val
                        prev.back()->ptrs.insert(prev.back()->ptrs.begin() + (i + 1), new Node<T>);

                        // link the next node
                        prev.back()->ptrs[i+1]->ptrs = current->ptrs;
                        if (!current->ptrs.empty()) current->ptrs.pop_back();
                        current->ptrs.push_back(prev.back()->ptrs[i+1]);

                        // move the value at index and beyond to the new node's values
                        for (size_t j = index; j < current->vals.size(); j++) {
                            prev.back()->ptrs[i+1]->vals.push_back(current->vals[j]);
                        }
                        // pop the values from current for vals.size-index
                        for (size_t j = index; j <= current->vals.size(); j++) {
                            current->vals.pop_back();
                        }
                        // the node has been fixed
                        fixed = true;
                        break;
                    }
                }
                // if the item needs to be inserted after the last element then
                // it would not have been fixed eariler and will be inserted at the back now
                if (!fixed){
                    // insert the number at the
                    prev.back()->vals.push_back(num);
                    prev.back()->ptrs.push_back(new Node<T>);

                    // link the next node
                    prev.back()->ptrs.back()->ptrs = current->ptrs;
                    if (!current->ptrs.empty()) current->ptrs.pop_back();
                    current->ptrs.push_back(prev.back()->ptrs.back());

                    // move the value at index and beyond to the new node's values
                    for (size_t j = index; j < current->vals.size(); j++) {
                        prev.back()->ptrs.back()->vals.push_back(current->vals[j]);
                    }
                    // pop the values from current for vals.size-index
                    for (size_t j = index; j <= current->vals.size(); j++) {
                        current->vals.pop_back();
                    }
                }
            } else {
                // if this level has nothing that needs to be fixed pop and check the next
                prev.pop_back();
            }
            fixed = false;
        }
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
        int i = 0;
        while (i < current->vals.size() && val >= current->vals[i]) {
            if (val == current->vals[i])
                return true;
            ++i;
        }
        current = current->ptrs[i];
    }
    for (int i = 0; i < current->vals.size() && !found; i++) {
        if (val == current->vals[i]){
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
    while (ptr) {
        for (int i = 0; i < ptr->vals.size(); ++i)
            out << ptr->vals[i] << " ";
        if (ptr->ptrs.empty())
            ptr = nullptr;
        else
            ptr = ptr->ptrs[0];
    }
    return out;
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
