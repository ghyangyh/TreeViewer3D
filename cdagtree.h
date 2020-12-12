#ifndef CDAGTREE_H
#define CDAGTREE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include <Eigen/Dense>

template<typename T>
struct CDAGNode {
    // the 3d position
    T m_x;
    T m_y;
    T m_z;

    // the radius
    T m_radius;

    // the branch level this node belongs to
    int m_branch_level;

    // parent and child nodes
    int m_num_children;
    std::shared_ptr<CDAGNode> m_parent_node_ptr;
    std::vector<std::shared_ptr<CDAGNode>> m_child_nodes;
};


template<typename T>
class CBranch {
public:
    CBranch():m_branch_level(-1){

    }
    ~CBranch(){}
    CBranch(const CBranch& aCopy):m_branch_level(aCopy.m_branch_level),m_branch_nodes(aCopy.m_branch_nodes){

    }
    CBranch& operator=(const CBranch& aRhs){
        if(*this != &aRhs) {
            m_branch_level = aRhs.m_branch_level;
            m_branch_nodes.clear();
            m_branch_nodes = aRhs.m_branch_nodes;
        }
        return (*this);
    }
public:
    void set_branch_level(int aLevel) {
        m_branch_level = aLevel;
    }
    void add_node(const std::shared_ptr<CDAGNode<T>>& aNodePtr) {
        m_branch_nodes.push_back(aNodePtr);
    }
private:
    int m_branch_level;
    std::vector<std::shared_ptr<CDAGNode<T>>> m_branch_nodes;
};

template<typename T>
class CBranchLevelSet {
public:
    CBranchLevelSet(){}
    ~CBranchLevelSet(){}
    CBranchLevelSet(const CBranchLevelSet& aCopy):m_branch_array(aCopy.m_branch_array){

    }
    CBranchLevelSet& operator=(const CBranchLevelSet& aRhs) {
        if(*this != &aRhs) {
            m_branch_array = aRhs.m_branch_array;
        }
        return (*this);
    }
public:
    void add_branch(const std::shared_ptr<CBranch<T>>& aBranchPtr) {
        m_branch_array.push_back(aBranchPtr);
    }
private:
    std::vector<std::shared_ptr<CBranch<T>>> m_branch_array;
};

template<typename T>
class CDAGTree
{
public:
    CDAGTree();
    ~CDAGTree();
    // Shallow copys?
    CDAGTree(const CDAGTree& aCopy);
    CDAGTree& operator=(const CDAGTree& aRhs);
public:
    /*
     * Load a tree file. A Directed Acylic Graph (DAG) is built.
     * The graph takes the adjacency-list represetation.
     * Returns true if the tree file is read successfully, otherwise false.
    */
    bool load_tree_file(const std::string& aFileName);
    /*
     * Extract branches from the tree graph.
     * The branches are extracted by level. E.g. the trunk branch is at level 1,
     * and all the branches directly connected to the trunk branch is at level 2, so on so forth...
    */
    void extract_branches();

    size_t get_total_num_of_nodes() const {
        return m_node_array.size();
    }
protected:
    void build_tree_graph_recursive(std::ifstream& inputs, std::shared_ptr<CDAGNode<T>>& aParentNodePtr, int aChildrenNum);
    void extract_branch_at_level(int aLevel, const std::shared_ptr<CDAGNode<T>>& aBranchRootNodePtr, CBranch<T>& aBranch);
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
CDAGTree<T>::CDAGTree(const CDAGTree<T>& aCopy) : m_node_array(aCopy.m_node_array){

}

template<typename T>
CDAGTree<T>& CDAGTree<T>::operator=(const CDAGTree<T>& aRhs) {
    if(*this != &aRhs) {
        m_node_array.clear();
        m_node_array = aRhs.m_node_array;
    }
    return (*this);
}

template<typename T>
bool CDAGTree<T>::load_tree_file(const std::string& aFileName) {
    std::ifstream inputs(aFileName);
    if(inputs.is_open()){
        // create the root node
        std::shared_ptr<CDAGNode<T>> root_node_ptr(new CDAGNode<T>());
        root_node_ptr->m_parent_node_ptr = nullptr;
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
        child_node_ptr->m_parent_node_ptr = aParentNodePtr;
        m_node_array.push_back(child_node_ptr);
        // recursively create the child nodes for this node
        build_tree_graph_recursive(inputs, child_node_ptr, child_node_ptr->m_num_children);
    }

}

template<typename T>
void CDAGTree<T>::extract_branch_at_level(int aLevel, const std::shared_ptr<CDAGNode<T>>& aBranchRootNodePtr, CBranch<T>& aBranch) {
    typedef Eigen::Matrix<T, 3, 1> Vector3T;
    if(aLevel == 1) { // the trunk is at level 1
        Vector3T a_up_dir(0, 1, 0);
        if(aBranchRootNodePtr->m_child_nodes.empty())
            return;
        // find the internode between the child nodes that is in a
        // direction closest to the up dir;
        std::shared_ptr<CDAGNode<T>> a_branch_node_ptr{};
        T min_abs_dot_value{};
        Vector3T a_root_pos(aBranchRootNodePtr->m_x, aBranchRootNodePtr->m_y, aBranchRootNodePtr->m_z);
        for(const auto& n : aBranchRootNodePtr->m_child_nodes) {
            if(n->m_branch_level > 0) // the node is already handled
                continue;
            Vector3T a_dir = Vector3T(n->m_x, n->m_y, n->m_z) - a_root_pos;
            a_dir.normalize();
            T a_dot_value = abs(a_up_dir.dot(a_dir));
            if(a_dot_value < min_abs_dot_value){
                min_abs_dot_value = a_dot_value;
                a_branch_node_ptr = n;
            }
        }
        a_branch_node_ptr->m_branch_level = aLevel;
        aBranch.add_node(a_branch_node_ptr);

        // Recursively extrat next branch node from this node
        extract_branch_at_level(aLevel, a_branch_node_ptr, aBranch);
    } else { // for other branches

    }
}

#endif // CDAGTREE_H
