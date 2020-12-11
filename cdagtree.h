#ifndef CDAGTREE_H
#define CDAGTREE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

template<typename T>
struct CDAGNode {
    T m_x;
    T m_y;
    T m_z;
    T m_radius;
    int m_num_children;
    int m_parent_node;
    //std::vector<int> m_child_nodes;
    std::vector<std::shared_ptr<CDAGNode>> m_child_nodes;
};


template<typename T>
class CDAGTree
{
public:
    CDAGTree();
    ~CDAGTree();
public:
    /*
     * Load a tree file. A Directed Acylic Graph (DAG) is built.
     * The graph takes the adjacency-list represetation.
     * Returns true if the tree file is read successfully, otherwise false.
    */
    bool load_tree_file(const std::string& aFileName);
protected:
    void build_tree_graph_recursive(std::ifstream& inputs, std::shared_ptr<CDAGNode<T>>& aParentNodePtr, int aChildrenNum);
private:
    std::vector<std::shared_ptr<CDAGNode<T>>> m_node_array;
};

template<typename T>
CDAGTree<T>::CDAGTree() {

}

template<typename T>
CDAGTree<T>::~CDAGTree() {

}

template<typename T>
bool CDAGTree<T>::load_tree_file(const std::string& aFileName) {
    std::ifstream inputs(aFileName);
    if(inputs.is_open()){
        // create the root node
        std::shared_ptr<CDAGNode<T>> root_node_ptr(new CDAGNode<T>());
        std::string str_line;
        std::getline(inputs, str_line, '\n');

        // the first line of the the input file should not be an empty line
        if(!str_line.empty()) {
            std::stringstream ss(str_line);
            ss >> root_node_ptr->m_x >> root_node_ptr->m_y >> root_node_ptr->m_z
               >> root_node_ptr->m_radius >> root_node_ptr->m_num_children;
            m_node_array.push_back(root_node_ptr);
        } else {
            std::cerr << "Error in reading the tree file: the first line should be a non-empty line!\n";
            return false;
        }

        build_tree_graph_recursive(inputs, root_node_ptr, root_node_ptr->m_num_children);

        return true;
    }
    return false;
}

template<typename T>
void CDAGTree<T>::build_tree_graph_recursive(std::ifstream& inputs, std::shared_ptr<CDAGNode<T>>& aParentNodePtr, int aChildrenNum) {
    if(inputs.eof())
        return ;
    // create the child nodes for the current parent node
    for(int i = 0; i < aChildrenNum; ++i) {
        std::string str_line;
        std::getline(inputs, str_line, '\n');
        std::stringstream ss(str_line);
        std::shared_ptr<CDAGNode<T>> child_node_ptr(new CDAGNode<T>());
        ss >> child_node_ptr->m_x >> child_node_ptr->m_y >> child_node_ptr->m_z
           >> child_node_ptr->m_radius >> child_node_ptr->m_num_children;
        aParentNodePtr->m_child_nodes.push_back(child_node_ptr);
        m_node_array.push_back(child_node_ptr);
        // recursively create the child nodes for this node
        build_tree_graph_recursive(inputs, child_node_ptr, child_node_ptr->m_num_children);
    }

}


#endif // CDAGTREE_H
