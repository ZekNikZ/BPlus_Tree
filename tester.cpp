#include <iostream>
#include <sstream>
#include <cassert>
#include "BPlusTree.h"

using namespace std;

int main() {
    BPlusTree<int> tree = BPlusTree<int>::makeTestTree();

    ostringstream os;
    os << tree;

    int x;
    istringstream is(os.str());
    while (is >> x) {
        cout << (tree.contains(x) ? "Contains" : "Doesn't contain");
        cout << " " << x << endl;
    }

    int nums[] = {872, 0, 8937575, 389247, 897123, 111111};
    for (int num : nums) {
        cout << (tree.contains(num) ? "Contains" : "Doesn't contain");
        cout << " " << num << endl;
    }

    // copy constructor
    auto copy = tree;
    copy.insert(1337);
    assert(!tree.contains(1337) && copy.contains(1337));
    // copy assign
    copy = tree;
    copy.insert(1111);
    assert(!tree.contains(1111) && copy.contains(1111));

    // move constructor
    auto moved = move(tree);
    moved.insert(1234);
    assert(moved.contains(1234) && !copy.contains(1234));

    // move assignment
    moved = move(copy);


    return 0;
}
