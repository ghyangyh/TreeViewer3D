#include "ctreeskeleton.h"
#include "GL/glew.h"

#include <vector>
using namespace std;

CTreeSkeleton::CTreeSkeleton(const shared_ptr<CDAGTree<float>>& aTreePtr)
{
    // reserve memories for the arrays
    m_first_indices.reserve(aTreePtr->get_total_num_of_branches());
    m_count_vertices.reserve(aTreePtr->get_total_num_of_branches());
    m_vertex_positions.reserve(3*aTreePtr->get_total_num_of_nodes());

    // create the tree skeleton
    create_tree_skeleton(aTreePtr);

    // create the vertex array object and vertex buffer object
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float)*m_vertex_positions.size(),
                 m_vertex_positions.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

CTreeSkeleton::~CTreeSkeleton(){
    m_vertex_positions.clear();
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_vao);
}


void CTreeSkeleton::draw() {
    glBindVertexArray(m_vao);
    //glDrawArrays(GL_LINE_STRIP, 0, m_total_vertices);
    glMultiDrawArrays(GL_LINE_STRIP,
                      m_first_indices.data(),
                      m_count_vertices.data(),
                      m_first_indices.size()
                      );
}

void CTreeSkeleton::draw_to_level(int aLevel) {

}

//void CTreeSkeleton::create_tree_skeleton(const shared_ptr<CDAGTree<float> > &aTreePtr) {
//    unsigned branch_count(0);
//    const vector<CBranchLevelSet<float>>& branch_set = aTreePtr->get_branches();
//    glBindVertexArray(m_vao);
//    // the branches with the same tree level are stored in a branch set
//    for(const auto& bs : branch_set){
//        const vector<shared_ptr<CBranch<float>>>& branch_array = bs.get_branch_array();
//        // iterate through all the branches in the smae branch set
//        for(const auto& b : branch_array) {
//            vector<float> vertex_positions(b->get_branch_nodes_nums()*3);
//            for(const auto& n : b->get_branch_nodes()) {
//                vertex_positions.push_back(n->m_x);
//                vertex_positions.push_back(n->m_y);
//                vertex_positions.push_back(n->m_z);
//                ++m_total_vertices;
//            }
//            // send the branch data to the OpenGL buffer
//            glBindBuffer(GL_ARRAY_BUFFER, m_vbos[branch_count]);
//            glBufferData(GL_ARRAY_BUFFER,
//                         sizeof(float)*vertex_positions.size(),
//                         vertex_positions.data(),
//                         GL_STATIC_DRAW);
//            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
//            glEnableVertexAttribArray(0);
//        }
//    }
//    glBindVertexArray(0);
//}

void CTreeSkeleton::create_tree_skeleton(const shared_ptr<CDAGTree<float> > &aTreePtr) {
    int vertex_count(0);
    const vector<CBranchLevelSet<float>>& branch_set = aTreePtr->get_branches();
    // the branches with the same tree level are stored in a branch set
    // iterate through each branch set (bs) at different levels
    for(const auto& bs : branch_set){
        const vector<shared_ptr<CBranch<float>>>& branch_array = bs.get_branch_array();
        // iterate through all the branches in the smae branch set
        for(const auto& b : branch_array) {
            // the start index of the first node in the current branch b
            m_first_indices.push_back(vertex_count);
            // the number of branch nodes for the current branch b
            m_count_vertices.push_back(int(b->get_branch_nodes_nums()));

            // iterate through all the branch nodes in the current branch
            for(const auto& n : b->get_branch_nodes()) {
                m_vertex_positions.push_back(n->m_x);
                m_vertex_positions.push_back(n->m_y);
                m_vertex_positions.push_back(n->m_z);
                ++vertex_count;
            }
        }
    }
}
