#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <vector>
#include <ostream>
#include <cassert>
#include <stack>

using namespace std;

template<typename T>
class BPlusTree {
    friend class BPlusTreeRenderer;

private:
    class Node {
        friend class BPlusTree;
        friend class BPlusTreeRenderer;
        friend BPlusTree BPlusTree::makeTestTree();

    public:
        enum NodeType {
            KEY,
            DATA
        };

        explicit Node(NodeType type = DATA) : type{type} {}
        const vector<Node*>& getPointers() const { return ptrs; }
        NodeType getType() const {return type; }
    private:
        NodeType type;       // type of noiide
        vector<Node *> ptrs; // pointers (KEY: pointers to other nodes, DATA: pointer to next data node)
        vector<T> vals;      // values (KEY: keys, DATA: data)
    };

    size_t M;    // order of table
    size_t L;    // amount of data in data nodes
    Node* root;  // root
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

// Getters
    // get M
    size_t getM() const;
    // does the tree contain val?
    bool contains(const T & val) const;
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
    stack<Node*> toDelete;
    toDelete.push(root);
    while (!toDelete.empty()) {
        Node* node = toDelete.top();
        toDelete.pop();
        if (node->getType() == Node::KEY) {
            for (Node* n : node->getPointers()) {
                toDelete.push(n);
            }
        }
        delete node;
    }
}

template<typename T>
BPlusTree<T>::BPlusTree(const BPlusTree &other) {
    assert(NOT_IMPLEMENTED);
}

template<typename T>
BPlusTree<T>::BPlusTree(BPlusTree &&other) {
    assert(NOT_IMPLEMENTED);
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree &other) {
    assert(NOT_IMPLEMENTED);
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator=(BPlusTree &&other) {
    assert(NOT_IMPLEMENTED);
}

//TODO: change prev to a stack
template<typename T>
void BPlusTree<T>::insert(const T &val) {
    bool insert = false;
    if (root == nullptr){
        root = new Node;
        root->vals.push_back(val);
    } else {
        Node* ptr = root;
        vector<Node*> prev;
        prev.push_back(ptr);
        while (ptr->getType() == Node::KEY){
            
            for (int i = 0; i < ptr->vals.size(); i++) {
                if (val < ptr->vals[i]){
                    ptr = ptr->ptrs[i];
                    break;
                }
            }
            if (ptr == prev.back()){
                ptr = ptr->ptrs[ptr->vals.size()];
            }
            prev.push_back(ptr);
        }
        for (int i = 0; i < ptr->vals.size() && !insert; i++) {
            if (val < ptr->vals[i]){
                ptr->vals.insert(ptr->vals.begin() + i, val);
                insert = true;
            }
        }
        if (!insert){
            ptr->vals.push_back(val);
        }
        // this part is buggy... very buggy...
        bool fixed = false;
        while(!prev.empty()){
            
            if (prev.back()->ptrs.size() > M){
                size_t index = M / 2;
                // get the value of the num pushed to a key
                T num = ptr->vals[index];
                // save the initial value of pointer and of the last visited
                Node* init = ptr, *initBack = prev.back();
                if (root == initBack){
                    root = new Node;
                    root->type = Node::KEY;
                    root->ptrs.push_back(initBack);
                    ptr = root;
                    prev.insert(prev.begin(), root);
                }
                prev.pop_back();
                    
                for (int i = 0; i <= initBack->vals.size(); i++) {
                    if (prev.back()->ptrs.size() == 1 || prev.back()->vals[i] >= num){
                        fixed = true;
                        prev.back()->vals.insert(prev.back()->vals.begin() + i, num);
                        prev.back()->ptrs.insert(prev.back()->ptrs.begin() + (i + 1), new Node);
                        prev.back()->type = Node::KEY;
                        
                        for (size_t j = index; j < init->vals.size(); j++) {
                            prev.back()->ptrs[i+1]->vals.push_back(init->vals[j]);
                        }
                        for (size_t j = index; j <= init->vals.size(); j++) {
                            init->vals.pop_back();
                        }
                        break;
                    }
                }
            } else if (prev.back()->vals.size() > L){
                size_t index = L / 2 + 1;
                // get the value of the num pushed to a key
                T num = ptr->vals[index];
                // save the initial value of pointer and of the last visited
                Node* init = ptr, *initBack = prev.back();
                
                if (root == initBack){
                    root = new Node;
                    root->type = Node::KEY;
                    root->ptrs.push_back(initBack);
                    ptr = root;
                    prev.insert(prev.begin(), root);
                }
                    // maybe move this so it is done each time?
                ptr = prev.back();
                prev.pop_back();
                    
                for (int i = 0; i < initBack->vals.size(); i++) {
                    /// currect bug is here when the keys are the part that need to be resized
                    if (prev.back()->vals.size() == 0 || prev.back()->vals[i] >= num){
                        fixed = true;
                        // these need to be fixed so that it
                        // this prev.back() is not pointing to the correct value
                        prev.back()->vals.insert(prev.back()->vals.begin() + i, num);
                        prev.back()->ptrs.insert(prev.back()->ptrs.begin() + (i + 1), new Node);
                        prev.back()->type = Node::KEY;
                        
                        for (size_t j = index; j < init->vals.size(); j++) {
                            prev.back()->ptrs[i+1]->vals.push_back(init->vals[j]);
                        }
                        for (size_t j = index; j < init->vals.size(); j++) {
                            init->vals.pop_back();
                        }
                        break;
                    }
                }
            } else {
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
}

template<typename T>
void BPlusTree<T>::makeEmpty() {
    stack<Node*> toDelete;
    toDelete.push(root);
    while (!toDelete.empty()) {
        Node* node = toDelete.top();
        toDelete.pop();
        if (node->getType() == Node::KEY) {
            for (Node* n : node->getPointers()) {
                toDelete.push(n);
            }
        }
        delete node;
    }
    root = nullptr;
}

template<typename T>
size_t BPlusTree<T>::getM() const {
    return M;
}

template<typename T>
bool BPlusTree<T>::contains(const T &val) const {
    assert(NOT_IMPLEMENTED);
}

template<typename T>
bool BPlusTree<T>::isEmpty() const {
    return root == nullptr;
}

template<class Key>
ostream &operator<<(ostream &out, const BPlusTree<Key> &tree) {
    assert(NOT_IMPLEMENTED);
}

template <class T>
BPlusTree<T> BPlusTree<T>::makeTestTree() {
    auto bottom8 = new Node(Node::DATA);
    bottom8->vals = {987, 1597, 9999};
    bottom8->ptrs = {nullptr};
    auto bottom7 = new Node(Node::DATA);
    bottom7->vals = {377, 610};
    bottom7->ptrs = {bottom8};
    auto bottom6 = new Node(Node::DATA);
    bottom6->vals = {144, 233};
    bottom6->ptrs = {bottom7};
    auto bottom5 = new Node(Node::DATA);
    bottom5->vals = {55, 89};
    bottom5->ptrs = {bottom6};
    auto bottom4 = new Node(Node::DATA);
    bottom4->vals = {21, 34};
    bottom4->ptrs = {bottom5};
    auto bottom3 = new Node(Node::DATA);
    bottom3->vals = {8, 13};
    bottom3->ptrs = {bottom4};
    auto bottom2 = new Node(Node::DATA);
    bottom2->vals = {3, 5};
    bottom2->ptrs = {bottom3};
    auto bottom1 = new Node(Node::DATA);
    bottom1->vals = {1, 2};
    bottom1->ptrs = {bottom2};

    auto midLeft = new Node(Node::KEY);
    midLeft->vals = {3, 8};
    midLeft->ptrs = {bottom1, bottom2, bottom3};
    auto midRight = new Node(Node::KEY);
    midRight->vals = {55, 144, 377, 987};
    midRight->ptrs = {bottom4, bottom5, bottom6, bottom7, bottom8};

    auto top = new Node(Node::KEY);
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
