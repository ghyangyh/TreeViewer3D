#ifndef GL_UTILITY_H
#define GL_UTILITY_H

/*
 * This module contains the utitlites to setup shader programs and textures
 * in opengl programming.
 * Author: Yinhui Yang
 * Zhejiang A&F University
*/

#include <string>

#include "opencv2/core.hpp"

/*
* Utilities for textures
*/

/*
 * Load an image from file.
 * aFileName: the image file path name
 * aTextureImage: the returned image data
 * Returns true if the image is successfully read, otherwise false
*/
bool load_image(const std::string& aFileName, cv::Mat& aTextureImage);

/*
 * Setup a opengl's texture unit for the given texture image.
 * aImageFile: a path to the texture image
 * Returns the texture id if everything goes ok, otherwise return -1 to indicate a failure.
*/
int set_texture(const std::string& aImageFile);

/*
* Utilities for shaders
*/

/*
 * Load a shader source file and store the file contents into a string.
 * aFileName: a path to the shader source file
 * aShaderSrcString: the returned string containing the shader source
 * Returns true if everything goes ok, otherwise return false.
 */
bool load_shader_file(const std::string& aFileName, std::string& aShaderSrcString);

/*
 * Create a opengl shader program from a vertex shader and a fragment shader.
 * aVertexShaderSrc: a path to the vertex shader source file
 * aFragmentShaderSrc: a path to the fragment shader source file
 * Returns the shader program id if everything goes ok, otherwise returns -1 to indicate a failure.
*/
int create_shader_program(const std::string& aVertexShaderSrc, const std::string& aFragmentShaderSrc);



#endif
