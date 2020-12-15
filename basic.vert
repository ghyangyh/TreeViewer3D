#version 330 core
layout(location=0) in vec3 vPosition;

//out vec3 fragment_color;

uniform mat4 proj,view,model;

void main()
{
	vec4 gl_Position = proj*view*model*vec4(vPosition, 1.0);
}