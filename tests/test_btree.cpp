#include "../structures/trees/btree.hpp"
#include <vector>
#include <stdlib.h>
#include <algorithm>
using namespace BTree;

int main(){
    Tree<int> t(10);
    t.Insert(8);
    t.Insert(9);
    t.Insert(10);
    t.Insert(15);
    t.Insert(16);
    t.Insert(17);
    t.Insert(18);
    t.Insert(20);
    t.Insert(23);

    cout << "The B-tree is: ";
    t.Scan();

    int k = 15;
    (t.Search(k) != nullptr) ? cout << endl
                    << k << " is found"
                : cout << endl
                    << k << " is not Found";

    k = 2;
    (t.Search(k) != nullptr) ? cout << endl
                    << k << " is found"
                : cout << endl
                    << k << " is not Found\n";


    t.Delete(18);
    t.Scan();
    t.Insert(255);
    t.Insert(99);
    t.Insert(120);
    t.Delete(99);
    t.Scan();
    return 0;
}