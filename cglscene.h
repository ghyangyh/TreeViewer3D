#ifndef CGLSCENE_H
#define CGLSCENE_H

#include "GLUtilities/camera.h"
#include "Eigen/Dense"

/*
 * The CGLScene represents a typical 3D Scene which is to be
 * rendered in OpenGL.
 * This class will provide interfaces to setup a 3d scene:
 *  setup the opengl rendering context
 *  setup a virtual camera for 3d navigation
 *  setup 3d objects
 *  setup simple lighting effects
 *  setup keyboard and mouse interactions
*/
class CGLScene
{
public:
    CGLScene(float aSceneScale);
    ~CGLScene();
    CGLScene(const CGLScene&)=delete;
    CGLScene& operator=(const CGLScene&)=delete;
public:
    static void set_framebuffer_size(int width, int height);
    void setup(int* argc, char** argv);
    void render();
protected:
    void init();
    static void reshape(int w, int h);
    static void display();
    static void keyboard(unsigned char key, int x, int y);
    static void mouse_input(int button, int state, int x, int y);
    static void mouse_motion(int x, int y);
private:
    float m_scene_scale;
    static int m_framebuffer_width;
    static int m_framebuffer_height;
    static int m_mouse_pos_x, m_mouse_pos_y;
    Eigen::Matrix4f m_model_mat, m_view_mat, m_proj_mat;
    int m_model_loc, m_view_loc, m_proj_loc;
    int m_shader_program;
    Camera m_fps_camera;
};

#endif // CGLSCENE_H
