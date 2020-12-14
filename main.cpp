#include <iostream>

using namespace std;

#include "cdagtree.h"

int main()
{
    CDAGTree<float> a_tree;
    if(a_tree.load_tree_file("/home/yinhui/Projects/Qt/Tree3DViewer/TestData/Tree1.tree")) {
        cout << "The tree file is read successfully!\n";
        cout << "Total number of tree nodes: " << a_tree.get_total_num_of_nodes() << endl;
        a_tree.extract_branches();
        cout << "Total number of tree branch levels: " << a_tree.get_total_num_of_branch_levels() << endl;
    } else {
        cout << "Failed read the tree file\n";
    }

    return 0;
}
