#ifndef CDAGTREE_H
#define CDAGTREE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <limits>

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

/*
 * The branch consists of a number of nodes.
*/
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
    std::vector<std::shared_ptr<CDAGNode<T>>>& get_branch_nodes() {
        return m_branch_nodes;
    }
    const std::vector<std::shared_ptr<CDAGNode<T>>>& get_branch_nodes() const {
        return m_branch_nodes;
    }
private:
    int m_branch_level;                                         // the branch level, e.g. the trunk has level 1.
    std::vector<std::shared_ptr<CDAGNode<T>>> m_branch_nodes;   // store all the nodes belongs to this branch by shared pointers.
};

/*
 * Store all the branches having the same level id.
*/
template<typename T>
class CBranchLevelSet {
public:
    CBranchLevelSet(){}
    ~CBranchLevelSet(){}
    CBranchLevelSet(const CBranchLevelSet& aCopy):m_branch_array(aCopy.m_branch_array){

    }
    CBranchLevelSet& operator=(const CBranchLevelSet& aRhs) {
        if(*this != &aRhs) {
            m_branch_array.clear();
            m_branch_array = aRhs.m_branch_array;
        }
        return (*this);
    }
public:
    void add_branch(const std::shared_ptr<CBranch<T>>& aBranchPtr) {
        m_branch_array.push_back(aBranchPtr);
    }
    std::vector<std::shared_ptr<CBranch<T>>>& get_branch_array() {
        return m_branch_array;
    }
    const std::vector<std::shared_ptr<CBranch<T>>>& get_branch_array() const {
        return m_branch_array;
    }
private:
    std::vector<std::shared_ptr<CBranch<T>>> m_branch_array;
};

/*
 * The Directed-Acyclic Tree. A adjacency-list representation is adopted here.
*/
template<typename T>
class CDAGTree
{
    typedef Eigen::Matrix<T, 3, 1> Vector3t;
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
     * This is only a dirver function of a recursive extraction funtion.
    */
    void extract_branches();

    size_t get_total_num_of_nodes() const {
        return m_node_array.size();
    }

    size_t get_total_num_of_branch_levels() const {
        return m_branches_array.size();
    }
protected:
    /*
     * Build the tree graph from a parent node in a depth-first recursive way.
     * Params:
     * inputs: the input file stream containing the nodes information
     * aParentNodePtr: the pointer to the node where the tree nodes are to be handled
     * aChildNum: the number of child nodes of the current node.
     * The member array m_node_array is modified after calling this function.
    */
    void build_tree_graph_recursive(std::ifstream& inputs, std::shared_ptr<CDAGNode<T>>& aParentNodePtr, int aChildrenNum);

    /*
     * Extract all the branches from a parent node recursively.
     * Params:
     * aLevel: the branch level
     * aParentNodePtr: the parent node to start.
    */
    void extract_branches_recursive(int aLevel,
                                    std::vector<std::shared_ptr<CDAGNode<T>>>& aPreviousLevelBranch);

    /*
     * A utility function to extract the branche at a specified level.
     * For the trunk (level 1), all the nodes with their direction to the parent node
     * closest to the 'up direction' is treated as a candidate trunk node. The 'up direction'
     * of the trunk is set as the (0, 1, 0) direction in this implementation.
     * For other branches, the 'up direction' is defined as the direction from the first child node
     * to its parent node at the parent branch of the previous tree level.
     * Params:
     * aLevel: the branch level
     * aBranchRootNodePtr: a pointer to the branch root node
     * aUpDir: the up direction for the branch
     * aBranch: the branch containing all the extracted nodes.
    */
    void extract_branch_at_level(int aLevel,
                                 const std::shared_ptr<CDAGNode<T>>& aBranchRootNodePtr,
                                 const Vector3t& aUpDir,
                                 CBranch<T>& aBranch);
private:
    std::vector<std::shared_ptr<CDAGNode<T>>> m_node_array; // store all the tree nodes using shared pointers.
    std::vector<CBranchLevelSet<T>> m_branches_array;       // store all the branches at different levels
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
void CDAGTree<T>::extract_branch_at_level(int aLevel,
                                          const std::shared_ptr<CDAGNode<T>>& aBranchRootNodePtr,
                                          const Vector3t& aUpDir,
                                          CBranch<T>& aBranch) {
        if(aBranchRootNodePtr->m_child_nodes.empty())   // reach a leaf node
            return;

        // find the internode from the current root node to the child nodes
        // that is in a direction closest to the up dir;
        std::shared_ptr<CDAGNode<T>> a_branch_node_ptr{};
        T min_abs_dot_value = std::numeric_limits<T>::max();
        // set the root node's position
        Vector3t a_root_pos(aBranchRootNodePtr->m_x, aBranchRootNodePtr->m_y, aBranchRootNodePtr->m_z);

        // iterate through all the child nodes of the current node to find the branch node
        for(const auto& n : aBranchRootNodePtr->m_child_nodes) {
            if(n->m_branch_level > 0) // the node is already handled (default should be 0)
                continue;
            Vector3t a_dir = Vector3t(n->m_x, n->m_y, n->m_z) - a_root_pos;
            a_dir.normalize();
            T a_dot_value = abs(aUpDir.dot(a_dir));
            // record the node with the closet direction
            if(a_dot_value < min_abs_dot_value){
                min_abs_dot_value = a_dot_value;
                a_branch_node_ptr = n;
            }
        }

        // add the found branch node
        a_branch_node_ptr->m_branch_level = aLevel;
        aBranch.add_node(a_branch_node_ptr);

        // Recursively extrat next branch node from this node
        extract_branch_at_level(aLevel, a_branch_node_ptr, aUpDir, aBranch);
}

template<typename T>
void CDAGTree<T>::extract_branches_recursive(int aLevel,
                                             std::vector<std::shared_ptr<CDAGNode<T>>>& aPreviousLevelBranch
                                              ) {
    CBranchLevelSet<T> a_branch_set;
    // for each node of the current branch
    for(auto& p : aPreviousLevelBranch) {
        // for each child node of p, extract the branches rooted at p
        for(auto& n : p->m_child_nodes){
            // the node is alread assigned to an existing branch
            if(n->m_branch_level > 0) continue;

            // compute the up direction
            Vector3t a_up_dir;
            Vector3t parent_node_pos(p->m_x, p->m_y,p->m_z);
            Vector3t current_node_pos(n->m_x, n->m_y, n->m_z);
            a_up_dir = current_node_pos - parent_node_pos;
            a_up_dir.normalize();

            // extract a branch rooted at the node p
            std::shared_ptr<CBranch<T>> a_branch_ptr(new CBranch<T>());
            extract_branch_at_level(aLevel, p, a_up_dir, (*a_branch_ptr));
            a_branch_set.add_branch(a_branch_ptr);

            // recursively extract branches at the next level started from this node
            extract_branches_recursive(aLevel+1, a_branch_ptr->get_branch_nodes());
        }

    }

    m_branches_array.push_back(a_branch_set);
}

template<typename T>
void CDAGTree<T>::extract_branches() {
    // extract the trunk branch
    int a_level(1);
    std::shared_ptr<CBranch<T>> trunk_branch_ptr(new CBranch<T>());
    Vector3t a_up_dir(0, 1, 0);
    extract_branch_at_level(a_level, m_node_array[0], a_up_dir, (*trunk_branch_ptr));
    CBranchLevelSet<T> trunk_branch_set;
    trunk_branch_set.add_branch(trunk_branch_ptr);
    m_branches_array.push_back(trunk_branch_set);

    // extract other branchs started from the trunk
    extract_branches_recursive(a_level+1, trunk_branch_ptr->get_branch_nodes());
}


#endif // CDAGTREE_H
