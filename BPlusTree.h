#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <vector>

using namespace std;

template<typename T>
class BPlusTree {
private:
    class Node {
    private:
        enum NodeType {
            KEY,
            DATA
        };
        NodeType type;       // type of node
        vector<Node *> ptrs; // pointers (KEY: pointers to other nodes, DATA: pointer to next node)
        vector<T> vals;      // values (KEY: keys, DATA: data)
    };

    size_t M;    // order of table
    size_t L;    // amount of data in data nodes
    Node * root; // root
    size_t size; // current amount of data stored

public:
    // Constructors
    explicit BPlusTree(size_t M = 5);        // default constructor: L=M-1
    explicit BPlusTree(size_t M, size_t L);  // other constructor
    ~BPlusTree();                            // destructor

    // Big N
    BPlusTree(const BPlusTree & other);              // copy constructor
    BPlusTree(BPlusTree && other);                   // move constructor
    BPlusTree & operator=(const BPlusTree & other);  // copy assignment operator
    BPlusTree & operator=(BPlusTree && other);       // move assignment operator


    // Modifiers
    void insert(const T & val);  // insert into tree
    void remove(const T & val);  // remove from tree
    void makeEmpty();            // make the tree empty

    // Getters
    size_t getM() const;                 // get M
    bool contains(const T & val) const;  // does the tree contain val?
    bool isEmpty() const;                // is the tree empty? (root==nullptr)

    friend ostream & operator<<(ostream & out, const BPlusTree & tree); // print somehow
};

#endif // BPLUSTREE_H
