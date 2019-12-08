#include <iostream>
#include <sstream>
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

    return 0;
}
