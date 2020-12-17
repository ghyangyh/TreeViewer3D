#include "cglscene.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "GLUtilities/gl_utilis.h"
#include "GLUtilities/gl_logger.h"
#include "GLUtilities/transformation_3d.h"

#include <iostream>

// Initialize the static members
int CGLScene::m_framebuffer_width(800);
int CGLScene::m_framebuffer_height(600);
int CGLScene::m_mouse_pos_x(0.f);
int CGLScene::m_mouse_pos_y(0.f);
int CGLScene::m_model_loc(-1);
int CGLScene::m_view_loc(-1);
int CGLScene::m_proj_loc(-1);
int CGLScene::m_shader_program(-1);
Camera CGLScene::m_fps_camera(Eigen::Vector3f(0.f, 0.f, 0.f), Eigen::Vector3f(0.f, 0.f, 0.f), Eigen::Vector3f(0.f, 1.f, 0.f));
Eigen::Matrix4f CGLScene::m_model_mat = Eigen::Matrix4f::Identity();
Eigen::Matrix4f CGLScene::m_view_mat = Eigen::Matrix4f::Identity();
Eigen::Matrix4f CGLScene::m_proj_mat = Eigen::Matrix4f::Identity();
std::shared_ptr<CTreeSkeleton> CGLScene::m_tree_skeleton_ptr = nullptr;

static std::string VERTEX_SHADER_SOURCE = "/home/yinhui/Projects/Qt/Tree3DViewer/basc.vert";
static std::string FRAGMENT_SHADER_SOURCE = "/home/yinhui/Projects/Qt/Tree3DViewer/basc.frag";
static std::string TREE_FILE_PATH = "/home/yinhui/Projects/Qt/Tree3DViewer/TestData/Tree1.tree";

void CGLScene::set_framebuffer_size(int width, int height) {
    m_framebuffer_width = width;
    m_framebuffer_height = height;
}

CGLScene::CGLScene():
{

}

CGLScene::~CGLScene() {

}

void CGLScene::init() {
    // GLobal state
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    // Create the shader program
    m_shader_program = create_shader_program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
    if(m_shader_program == -1) {
        std::cerr << "ERROR: failed create shader program from given shader sources!\n";
        exit(1);
    }
    // Initialize the model, view and projection matrices
    m_view_mat = m_fps_camera.get_view_mat();
    m_proj_mat = perspective(60.f, float(m_framebuffer_width)/float(m_framebuffer_width), 1.f, 1000.f);

    // Setup the model, view and projection matrices in the OpenGL shader
    glUseProgram(m_shader_program);
    m_model_loc = glGetUniformLocation(m_shader_program, "model");
    if(m_model_loc == -1) {
        std::cerr << "ERROR:failed get the model location from the shader!\n";
        exit(1);
    }
    glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, m_model_mat.data());

    m_view_loc = glGetUniformLocation(m_shader_program, "view");
    if(m_view_loc == -1) {
        std::cerr << "ERROR:failed get the view location from the shader!\n";
        exit(1);
    }
    glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, m_view_mat.data());

    m_proj_loc = glGetUniformLocation(m_shader_program, "proj");
    if(m_proj_loc == -1) {
        std::cerr << "ERROR:failed get the projection location from the shader!\n";
        exit(1);
    }
    glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, m_proj_mat.data());

}

void CGLScene::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_shader_program);
    m_tree_skeleton_ptr->draw();
    glutSwapBuffers();
}

void CGLScene::reshape(int w, int h) {
    glViewport(0, 0, w, h);
    m_framebuffer_width = w;
    m_framebuffer_height = h;
    glUseProgram(m_shader_program);
    m_proj_mat = perspective(60.f, float(w)/float(h), 1.f, 1000.f);
    glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, m_proj_mat.data());
}

void CGLScene::keyboard(unsigned char key, int x, int y) {
    switch(key) {
    case 'w':
        m_fps_camera.move_foward();
        m_fps_camera.update_camera_system();
        m_view_mat = m_fps_camera.get_view_mat();
        glUseProgram(m_shader_program);
        glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, m_view_mat.data());
        glutPostRedisplay();
        break;
    case 's':
        m_fps_camera.move_backward();
        m_fps_camera.update_camera_system();
        m_view_mat = m_fps_camera.get_view_mat();
        glUseProgram(m_shader_program);
        glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, m_view_mat.data());
        glutPostRedisplay();
        break;
    case 'a':
        m_fps_camera.move_left();
        m_fps_camera.update_camera_system();
        m_view_mat = m_fps_camera.get_view_mat();
        glUseProgram(m_shader_program);
        glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, m_view_mat.data());
        glutPostRedisplay();
        break;
    case 'd':
        m_fps_camera.move_right();
        m_fps_camera.update_camera_system();
        m_view_mat = m_fps_camera.get_view_mat();
        glUseProgram(m_shader_program);
        glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, m_view_mat.data());
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void CGLScene::mouse_input(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        m_mouse_pos_x = x;
        m_mouse_pos_y = y;
    }
}

void CGLScene::mouse_motion(int x, int y) {
    int dx = x - m_mouse_pos_x;
    int dy = y - m_mouse_pos_y;
    m_mouse_pos_x = x;
    m_mouse_pos_y = y;
    m_fps_camera.update_orientation(float(dy), float(dx));
    m_view_mat = m_fps_camera.get_view_mat();
    glUseProgram(m_shader_program);
    glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, m_view_mat.data());
    glutPostRedisplay();
}

void CGLScene::create_tree_skeleton() {
    std::shared_ptr<CDAGTree<float>> a_tree_ptr(new CDAGTree<float>());
    if(a_tree_ptr->load_tree_file(TREE_FILE_PATH)) {
        std::cout << "The tree file is read successfully!\n";
        std::cout << "Total number of tree nodes: " << a_tree_ptr->get_total_num_of_nodes() << std::endl;
        a_tree_ptr->extract_branches();
        std::cout << "Total number of tree branch levels: " << a_tree_ptr->get_total_num_of_branch_levels() << std::endl;
        CBBox<float> tree_box;
        a_tree_ptr->compute_bounding_box(tree_box);
        // create a tree skeleton
        m_tree_skeleton_ptr.reset(new CTreeSkeleton(a_tree_ptr));
        float z_scale = tree_box.m_z_max - tree_box.m_z_min;
        m_fps_camera.set_camera_position(m_fps_camera.get_cam_pos() + Eigen::Vector3f(0.f, 0.f, 2.f*z_scale));

    } else {
        std::cout << "Failed read the tree file\n";
    }
}

void CGLScene::setup(int* argc, char** argv) {
    glutInit(argc, argv);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextVersion(4, 3);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(m_framebuffer_width, m_framebuffer_height);
    glutCreateWindow("Tree3DViewer");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse_input);
    glutMotionFunc(mouse_motion);

    glewExperimental = GL_TRUE;
    glewInit();

    // Setup the tree skeleton to be rendered
    create_tree_skeleton();

    // Initialize some global opengl states
    init();

    // Query and print some opengl informations
    restart_gl_log("gl_params.txt");
    log_gl_params("gl_params.txt");

    glutMainLoop();

}
