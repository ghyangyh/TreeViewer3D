#include "gl_utilis.h"

#include "GL/glew.h"


#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

bool load_image(const string& aFileName, cv::Mat& aTextureImage) {
	aTextureImage = cv::imread(aFileName, cv::IMREAD_UNCHANGED);
	if (aTextureImage.empty()) {
		cout << "ERROR: Failed load the texture image: " << aFileName << endl;
		return false;
	}
	else {
		// flip the image
		cv::flip(aTextureImage, aTextureImage, 0);
		// convert to RGBA
		cv::cvtColor(aTextureImage, aTextureImage, cv::COLOR_BGRA2RGBA);
		return true;
	}
}


int set_texture(const string& aImageFile) {
	cv::Mat a_texture_image;
	if (load_image(aImageFile, a_texture_image)) {
		unsigned int texture_id;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		int texture_width = a_texture_image.size[1];
		int texture_height = a_texture_image.size[0];
		cout << "Texture size: " << texture_width << ", " << texture_height << endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, a_texture_image.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
		return texture_id;
	}
	return -1;
}

bool load_shader_file(const string& aFileName, string& aShaderSrcString) {
	ifstream inputs(aFileName);
	if (inputs) {
		stringstream ss;
		ss << inputs.rdbuf();
		aShaderSrcString = ss.str();
		return true;
	}
	else {
		cout << "Failed loading the shader file: " << aFileName << endl;
		return false;
	}
}

int create_shader_program(const string& aVertexShaderSrc, const string& aFragmentShaderSrc) {
	ofstream log_outputs("gl_logger.txt");

	// Create a shader program
	unsigned shader_program_id = glCreateProgram();

	// Create the vertex and fragment shaders
	unsigned vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	unsigned fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// Load the shader sources from files
	string vertex_src_str, fragment_src_str;
	if (!load_shader_file(aVertexShaderSrc, vertex_src_str))
		return -1;
	if (!load_shader_file(aFragmentShaderSrc, fragment_src_str))
		return -1;

	// Send the shader sources to opengl
	const char* vertex_src_cstr = vertex_src_str.c_str();
	const char* fragment_src_cstr = fragment_src_str.c_str();
	glShaderSource(vertex_shader_id, 1, &(vertex_src_cstr), 0);
	glShaderSource(fragment_shader_id, 1, &(fragment_src_cstr), 0);

	// Compile shaders
	glCompileShader(vertex_shader_id); // Compile the vertex shader
	int success(0);
	char message_buf[256];
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader_id, 256, 0, message_buf);
		log_outputs << "Failed compiling the vertex shader: \n" << message_buf << endl;
		glDeleteShader(vertex_shader_id);
		return -1;
	}
	glCompileShader(fragment_shader_id); // Compile the fragment shader
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader_id, 256, 0, message_buf);
		log_outputs << "Failed compiling the fragment shader: \n" << message_buf << endl;
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
		return -1;
	}

	// Link shaders into a shader program
	glAttachShader(shader_program_id, vertex_shader_id);
	glAttachShader(shader_program_id, fragment_shader_id);
	glLinkProgram(shader_program_id);
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program_id, 256, 0, message_buf);
		log_outputs << "Failed linking the fragment shader: \n" << message_buf << endl;
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
		glDeleteProgram(shader_program_id);
		return -1;
	}

	// The shaders can be deleted after successfully linked into a program
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	return shader_program_id;
}