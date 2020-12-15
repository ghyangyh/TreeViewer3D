#include "cglscene.h"
#include "GL/freeglut.h"
#include "GL/glew.h"
#include "GLUtilities/gl_utilis.h"
#include "GLUtilities/gl_logger.h"
#include "GLUtilities/transformation_3d.h"

int CGLScene::m_framebuffer_width(800);
int CGLScene::m_framebuffer_height(600);
int CGLScene::m_mouse_pos_x(0.f);
int CGLScene::m_mouse_pos_y(0.f);

void CGLScene::set_framebuffer_size(int width, int height) {
    m_framebuffer_width = width;
    m_framebuffer_height = height;
}

CGLScene::CGLScene(float aSceneScale):
    m_scene_scale(aSceneScale),
    m_model_loc(-1), m_view_loc(-1), m_proj_loc(-1),
    m_shader_program(-1),
    m_fps_camera(Eigen::Vector3f(0.f, 0.f, m_scene_scale*2.f), Eigen::Vector3f(0.f, 0.f, 0.f), Eigen::Vector3f(0.f, 1.f, 0.f))
{

}

CGLScene::~CGLScene() {

}

void CGLScene::init() {

}

void CGLScene::display() {

}

void CGLScene::reshape(int w, int h) {

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

    init();

    // Query and print some opengl informations
    restart_gl_log("gl_params.txt");
    log_gl_params("gl_params.txt");

    glutMainLoop();

}
