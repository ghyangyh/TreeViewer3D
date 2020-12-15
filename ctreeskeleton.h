#ifndef CTREESKELETON_H
#define CTREESKELETON_H

#include <vector>
#include <memory>
#include "cdagtree.h"


/*
 * This class will create a 3d tree skeleton for rendering.
 * The tree skeleton is a collection of tree branches, which is
 * created from a directed-acylic graph.
 * The tree skeleton is rendered in OpenGL
*/

class CTreeSkeleton
{
public:
    CTreeSkeleton(const std::shared_ptr<CDAGTree<float>>& aTreePtr);
    ~CTreeSkeleton();
    CTreeSkeleton(const CTreeSkeleton& aCopy)=delete;
    CTreeSkeleton& operator=(const CTreeSkeleton& aRhs)=delete;
public:
    /*
     * Draw the entire tree seketon.
    */
    void draw();
    /*
     * Draw the tree skeleton's branches with level less than and equal to
     * the given level.
    */
    void draw_to_level(int aLevel);
    /*
     * Create a tree skeleton from the dagtree.
    */
    void create_tree_skeleton(const std::shared_ptr<CDAGTree<float>>& aTreePtr);
private:
    unsigned m_vao;
    unsigned m_vbo;
    std::vector<int> m_first_indices;
    std::vector<int> m_count_vertices;
    std::vector<float> m_vertex_positions;
//    std::vector<unsigned> m_vbos;
//    int m_total_vertices;
//    std::vector<unsigned> m_vertex_indices;
};

#endif // CTREESKELETON_H
