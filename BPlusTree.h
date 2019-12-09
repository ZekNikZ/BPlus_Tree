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
    size_t M;    // order of table
    size_t L;    // amount of data in data nodes
    Node<T> * root;  // root
    //TODO: Is the the amount of data stored at the bottom in the data nodes,
    // or for all nodes?
    // ^ Amount of Data ^
    size_t size; // current amount of data stored

    void remove (const T & key, Node<T> * ptr, Node<T> * node);

    Node<T> * rightSib(Node<T> * node);

    Node<T> * leftSib(Node<T> * node);

    Node<T> * par(Node<T> * node, Node<T> * n=nullptr);

public:
// Constructors
    /*
     Description: Default construcor for a B+ Tree
     Return: n/A
     Precondition: None
     Postcondition: A B+ tree exists with the order specified
     */
    explicit BPlusTree(size_t M = 5);
    /*
    Description: Alternate construcor for a B+ Tree
    Return: n/A
    Precondition: None
    Postcondition: A B+ tree exists with the order and datanode size specified
    */
    BPlusTree(size_t M, size_t L);
    /*
    Description: Destructor for B+ tree
    Return: n/A
    Precondition: The B+ tree has been constructed
    Postcondition: The B+ Tree is destructed and all allocated memory is deallocated
    */
    ~BPlusTree();

// Big N
    /*
    Description: Copy constructor for B+ tree
    Return: n/A
    Precondition: "other" has already been constructed
    Postcondition: The new B+ tree is created which is a perfect copy of "other"
    */
    BPlusTree(const BPlusTree & other);
    /*
    Description: Move constructor for B+ tree
    Return: n/A
    Precondition: "other" has already been constructed
    Postcondition: The new B+ tree is created and passed the root of "other".
    */
    BPlusTree(BPlusTree && other);
    /*
    Description: Copy assignment operator for B+ tree
    Return: a reference to the current B+ tree
    Precondition: This tree and "other" has already been constructed
    Postcondition: The current B+ tree is cleared and copies all the values from "other"
    */
    BPlusTree & operator=(const BPlusTree & other);
    /*
    Description: Move assignment operator for B+ tree
    Return: a reference to the current B+ tree
    Precondition: This tree and "other" has already been constructed
    Postcondition: The current B+ tree is cleared and now points to "other"'s root
    */
    BPlusTree & operator=(BPlusTree && other);


// Modifiers
    /*
    Description: Insert the passed value into the B+ tree
    Return: void
    Precondition: The B+ tree is constructed
    Postcondition: The new value is inserted as data into the new tree, re-balancing if necessary.
    */
    void insert(const T & val);
    /*
    Description: Removes the passed value from the B+ tree if it's present in the tree
    Return: void
    Precondition: The B+ tree is constructed
    Postcondition: If the value exists in the tree, it is removed and the tree is re-balanced
     and memory is de-allocated if necessary.
    */
    void remove(const T & val);
    /*
    Description: Removes all values from the B+ tree
    Return: void
    Precondition: The B+ tree is constructed
    Postcondition: All previous values in the tree are removed, and all allocated memory is de-allocated.
    */
    void makeEmpty();
    /*
    Description: Removes all values from the B+ tree and modifies the value of M
    Return: void
    Precondition: The B+ tree is constructed
    Postcondition: All values in the tree are removed, all memory is deallocated and M and L
     are modified to the new M and the new M - 1, respectively.
    */
    void setMAndClear(int m) {
        makeEmpty();
        this->M = m;
        this->L = m - 1;
    }
    /*
    Description: Removes all values from the B+ tree and modifies the value of M
    Return: void
    Precondition: The B+ tree is constructed
    Postcondition: All values in the tree are removed, all memory is deallocated and M and L
     are modified to the new M and the new M - 1, respectively.
    */
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
    makeEmpty();
}

template<typename T>
BPlusTree<T>::BPlusTree(const BPlusTree &other) {
    M = other.M;
    L = other.L;
    root = nullptr;
    size = 0; // changed when inserting
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
    other.root = nullptr; // no double free
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
        size = 0; // changed on inserts
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
        other.root = nullptr;
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
                        // make a new KEY node in previous' pointers at index
                        prev.back()->ptrs.insert(prev.back()->ptrs.begin() + (i + 1), new Node<T>(Node<T>::KEY));

                        // move the value at index and beyond to the new node's values
                        size_t ibvSize = initBack->vals.size();
                        for (size_t j = index + 1; j < ibvSize; j++) {
                            prev.back()->ptrs[i+1]->vals.push_back(initBack->vals[j]);
                        }
                        // pop the values from initBack for vals.size-index
                        for (size_t j = index + 1; j <= ibvSize; j++) {
                            initBack->vals.pop_back();
                        }
                        // move the pointers at index and beyond to the new node's pointers
                        size_t ibpSize = initBack->ptrs.size();
                        for ( size_t j = index + 1; j < ibpSize; j++){
                            prev.back()->ptrs[i+1]->ptrs.push_back(initBack->ptrs[j]);
                        }
                        // pop the pointers from initial back for ptrs.size-index
                        for (size_t j = index + 1; j < ibpSize; j++) {
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
                    size_t ibvSize = initBack->vals.size();
                    for (size_t j = index + 1; j < ibvSize; j++) {
                        prev.back()->ptrs.back()->vals.push_back(initBack->vals[j]);
                    }
                    // pop the values from initBack for vals.size-index
                    for (size_t j = index + 1; j <= ibvSize; j++) {
                        initBack->vals.pop_back();
                    }
                    // move the pointers at index and beyond to the new node's pointers
                    size_t ibpSize = initBack->ptrs.size();
                    for ( size_t j = index + 1; j < ibpSize; j++){
                        prev.back()->ptrs.back()->ptrs.push_back(initBack->ptrs[j]);
                    }
                    // pop the pointers from initial back for ptrs.size-index
                    for (size_t j = index + 1; j < ibpSize; j++) {
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
                        size_t cvSize = current->vals.size();
                        for (size_t j = index; j < cvSize; j++) {
                            prev.back()->ptrs[i+1]->vals.push_back(current->vals[j]);
                        }
                        // pop the values from current for vals.size-index
                        for (size_t j = index; j < cvSize; j++) {
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
                    size_t cvSize = current->vals.size();
                    for (size_t j = index; j < cvSize; j++) {
                        prev.back()->ptrs.back()->vals.push_back(current->vals[j]);
                    }
                    // pop the values from current for vals.size-index
                    for (size_t j = index; j < cvSize; j++) {
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
    // Ensure that there is data in the tree
    if (root == nullptr) {
        return;
    }

    // Navigate to the data node where the value is
    vector<Node<T> *> visitedNodes;
    Node<T> * ptr = root;
    int childIndex = 0;
    while (ptr->type == Node<T>::KEY) {
        size_t i = 0;
        for (; i < ptr->vals.size(); ++i) {
            if (val < ptr->vals[i]) {
                break;
            }
        }
        visitedNodes.push_back(ptr);
        ptr = ptr->ptrs[i];
        childIndex = i;
    }

    // Ensure the data is in the tree
    if (find(ptr->vals.begin(), ptr->vals.end(), val) == ptr->vals.end()) {
        return;
    }

    // Remove the value
    bool updateParent = false;
    if (ptr->vals[0] == val && ptr != root) {
        updateParent = true;
    }
    ptr->vals.erase(find(ptr->vals.begin(), ptr->vals.end(), val));

    // Check if we need to steal data from another data node
    bool updateRight = false, collapse = false;
    Node<T> * leftSiblingsParent = nullptr;
    T oldRightFront;
    if (ptr->vals.size() < L / 2 && ptr != root) {
        // Check if right sibling has excess data
        if (!ptr->ptrs.empty() && ptr->ptrs.front()->vals.size() > L/2) {
            updateRight = true;
            oldRightFront = ptr->ptrs.front()->vals.front();
            ptr->vals.push_back(oldRightFront);
            ptr->ptrs.front()->vals.erase(ptr->ptrs.front()->vals.begin());
            cout << "steal from right" << endl;
        } else {
            // Navigate to the left sibling
            Node<T> * leftSibling = nullptr;
            if (childIndex > 0) {
                leftSibling = visitedNodes.back()->ptrs[childIndex - 1];
            } else {
                auto leftPtr = ++visitedNodes.rbegin();
                if (leftPtr != visitedNodes.rend()) {
                    auto leftPtrIndex = find((*leftPtr)->ptrs.begin(), (*leftPtr)->ptrs.end(), *(leftPtr - 1));
                    while (leftPtrIndex == (*leftPtr)->ptrs.begin()) {
                        ++leftPtr;
                        if (leftPtr == visitedNodes.rend()) break;
                        leftPtrIndex = find((*leftPtr)->ptrs.begin(), (*leftPtr)->ptrs.end(), *(leftPtr - 1));
                    }
                    if (leftPtr != visitedNodes.rend()) {
                        leftSibling = *leftPtr;
                        leftSibling = leftSibling->ptrs.front();
                        while (leftSibling->type != Node<T>::DATA) {
                            leftSiblingsParent = leftSibling;
                            leftSibling = leftSibling->ptrs.back();
                        }
                    }
                }
            }

            // Check if left sibling has excess data
            cout << "leftSibling: " << leftSibling->vals.front() << endl;
            if (leftSibling && leftSibling->vals.size() > L / 2) {
                ptr->vals.insert(ptr->vals.begin(), leftSibling->vals.back());
                leftSibling->vals.pop_back();
                updateParent = true;
                cout << "steal from left" << endl;
            }
            // Otherwise, collapse
            else {
                collapse = true;
                cout << "collapse" << endl;
            }
        }
    }

    if (ptr == root && ptr->vals.size() == 0) {
        delete ptr;
        root = nullptr;
        return;
    }

    // Collapse and/or fix parents
    if (!collapse) {
        // Check if we need to update the parent
        if (updateParent) {
            auto parentPtr = visitedNodes.rbegin();
            for (size_t i = 1; i < (*parentPtr)->ptrs.size(); ++i) {
                (*parentPtr)->vals[i - 1] = (*parentPtr)->ptrs[i]->vals.front();
            }
            while(parentPtr != visitedNodes.rend() && find((*parentPtr)->vals.begin(), (*parentPtr)->vals.end(), val) == (*parentPtr)->vals.end()) {
                ++parentPtr;
            }
            if (parentPtr != visitedNodes.rend()) {
                for (auto & key : (*parentPtr)->vals) {
                    if (key == val) {
                        key = ptr->vals.front();
                        break;
                    }
                }
            }
        }

        // Check if we need to update the right parent
        if (updateRight && false) {
            Node<T> * rightSibling = nullptr, *rightSiblingsParent = nullptr;
            if (childIndex + 1 < visitedNodes.back()->ptrs.size()) {
                rightSiblingsParent = visitedNodes.back();
                rightSibling = visitedNodes.back()->ptrs[childIndex + 1];
            } else {
                auto rightPtr = ++visitedNodes.rbegin();
                auto rightPtrIndex = find((*rightPtr)->ptrs.begin(), (*rightPtr)->ptrs.end(), *(rightPtr - 1));
                while (rightPtrIndex == (*rightPtr)->ptrs.end() - 1) {
                    ++rightPtr;
                    if (rightPtr == visitedNodes.rend()) break;
                    rightPtrIndex = find((*rightPtr)->ptrs.begin(), (*rightPtr)->ptrs.end(), *(rightPtr - 1));
                }
                rightSibling = *rightPtr;
                rightSibling = rightSibling->ptrs.back();
                stack<Node<T> *> nodeStack;
                while (rightSibling->type != Node<T>::DATA) {
                    nodeStack.push(rightSibling);
                    rightSiblingsParent = rightSibling;
                    rightSibling = rightSibling->ptrs.front();
                }
                while (find(rightSiblingsParent->vals.begin(), rightSiblingsParent->vals.end(), oldRightFront) == rightSiblingsParent->vals.end()) {
                    rightSiblingsParent = nodeStack.top();
                    if (nodeStack.empty()) break;
                    nodeStack.pop();
                }
                if (find(rightSiblingsParent->vals.begin(), rightSiblingsParent->vals.end(), oldRightFront) == rightSiblingsParent->vals.end()) {
                    while(find((*rightPtr)->vals.begin(), (*rightPtr)->vals.end(), oldRightFront) == (*rightPtr)->vals.end()) {
                        --rightPtr;
                    }
                    rightSiblingsParent = *rightPtr;
                }
                rightSiblingsParent->vals.front() = rightSibling->vals.front();
            }
        }
    } else {
        Node<T>* parent = par(ptr);
        Node<T> * rightSibling = rightSib(ptr);
        Node<T> * leftSibling = leftSib(ptr);

        // Try merging with right
        if (rightSibling) {
            for (T & val : rightSibling->vals) {
                ptr->vals.push_back(val);
            }
            ptr->ptrs.front() = rightSibling->ptrs.front();

            Node<T> * rightParent = par(rightSibling);

            int i = 0;
            for (; i < rightSibling->vals.size(); ++i) {
                if (rightSibling->vals[i] > rightParent->vals.front()) {
                    break;
                }
            }

            delete(i - 1, rightSibling, rightParent);
        }
        // Try merging with left
        else {
            for (T & val : ptr->vals) {
                leftSibling->vals.push_back(val);
            }
            leftSibling->ptrs.front() = ptr->ptrs.front();

            int i = 0;
            for (; i < parent->vals.size(); ++i) {
                if (parent->vals[i] > ptr->vals.front()) {
                    break;
                }
            }

            delete(i - 1, ptr, parent);
        }
    }
}

template<typename T>
Node<T> * BPlusTree<T>::rightSib(Node<T> * node) {
    Node<T> * ptr = root;
    stack<pair<Node<T> *, int>> nodeStack;
    nodeStack.push({ptr, 0});

    // Go down
    while (ptr != node) {
        int i = 0;
        for (; i < ptr->vals.size(); ++i) {
            if (ptr->vals[i] > node->vals.front()) {
                ptr = ptr->ptrs[i];
                break;
            }
        }

        if (i == ptr->vals.size()) {
            ptr = ptr->ptrs.back();
        }

        nodeStack.push({ptr, i});
    }

    int height = nodeStack.size();

    // Go up
    ptr = nodeStack.top().first;
    int index = nodeStack.top().second;
    nodeStack.pop();

    while (!nodeStack.empty() && index + 1 >= nodeStack.top().first->ptrs.size()) {
        ptr = nodeStack.top().first;
        index = nodeStack.top().second;
        nodeStack.pop();
    }

    if (nodeStack.empty()) return nullptr;

    nodeStack.push({ptr, -1});
    ptr = ptr->ptrs[index + 1];

    // Go down
    while (nodeStack.size() != height) {
        nodeStack.push({ptr, -1});
        ptr = ptr->ptrs.front();
    }

    return ptr;
}

template<typename T>
Node<T> * BPlusTree<T>::leftSib(Node<T> * node) {
    Node<T> * ptr = root;
    stack<pair<Node<T> *, int>> nodeStack;
    nodeStack.push({ptr, 0});

    // Go down
    while (ptr != node) {
        int i = 0;
        for (; i < ptr->vals.size(); ++i) {
            if (ptr->vals[i] > node->vals.front()) {
                ptr = ptr->ptrs[i];
                break;
            }
        }

        if (i == ptr->vals.size()) {
            ptr = ptr->ptrs.back();
        }

        nodeStack.push({ptr, i});
    }

    int height = nodeStack.size();

    // Go up
    ptr = nodeStack.top().first;
    int index = nodeStack.top().second;
    nodeStack.pop();

    while (!nodeStack.empty() && index - 1 >= 0) {
        ptr = nodeStack.top().first;
        index = nodeStack.top().second;
        nodeStack.pop();
    }

    if (nodeStack.empty()) return nullptr;

    nodeStack.push({ptr, -1});
    ptr = ptr->ptrs[index - 1];

    // Go down
    while (nodeStack.size() != height) {
        nodeStack.push({ptr, -1});
        ptr = ptr->ptrs.back();
    }

    return ptr;
}

template<typename T>
Node<T>* BPlusTree<T>::par(Node<T> * node, Node<T> * n) {
    Node<T> * ptr = n;

    if (ptr == node) {
        return ptr;
    }

    int i = 0;
    for (; i < ptr->vals.size(); ++i) {
        if (ptr->vals[i] > node->vals.front()) {
            ptr = ptr->ptrs[i];
            break;
        }
    }
    if (ptr == n) {
        ptr = ptr->ptrs.back();
    }

    return par(node, ptr);
}

template<typename T>
void BPlusTree<T>::remove(const T & key, Node<T> * subtree, Node<T> * node) {
    // Delete the key and subtree
    auto keyLoc = find(node->vals.begin(), node->vals.end(), key);
    node->vals.erase(keyLoc - node->vals.begin());
    auto subtreeLoc = find(node->ptrs.begin(), node->ptrs.end(), subtree);
    delete *subtreeLoc;
    node->ptrs.erase(subtreeLoc - node->ptrs.begin());

    // If we still have enough keys, stop
    if (node->ptrs.size() <= M / 2) {
        Node<T>* parent = par(node);
        Node<T> * rightSibling = rightSib(node);
        Node<T> * leftSibling = leftSib(node);
        auto nodeLocInParent = find(parent->ptrs.begin(), parent->ptrs.end(), node);
        T & keyInParent = parent->keys[distance(parent->ptrs.begin(), nodeLocInParent)];

        // Try stealing from right
        if (rightSibling && rightSibling->ptrs.size() > M / 2 + 1) {
            // Move the leftmost pointer from the right sibling
            node->ptrs.push_back(rightSibling->ptrs.front());
            rightSibling->ptrs.erase(rightSibling->ptrs.begin());

            // Move the leftmost key through the parent
            node->vals.push_back(keyInParent);
            keyInParent = rightSibling->vals.front();
            rightSibling->vals.erase(rightSibling->vals.begin());
        }
        // Try stealing from left
        else if (leftSibling && leftSibling->ptrs.size() > M / 2 + 1) {
            // Move the leftmost pointer from the right sibling
            node->ptrs.insert(node->ptrs.begin(), leftSibling->ptrs.back());
            leftSibling->ptrs.pop_back();

            // Move the leftmost key through the parent
            node->vals.insert(node->vals.begin(), keyInParent);
            keyInParent = leftSibling->vals.back();
            leftSibling->vals.pop_back();
        }
        // Try merging with right
        else if (rightSibling) {
            node->vals.push_back(keyInParent);
            for (T & val : rightSibling->vals) {
                node->vals.push_back(val);
            }

            delete(keyInParent, *nodeLocInParent, parent);
        }
        // Try merging with left
        else {
            leftSibling->vals.push_back(keyInParent);
            for (T & val : node->vals) {
                leftSibling->vals.push_back(val);
            }

            delete(keyInParent, *nodeLocInParent, parent);
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
