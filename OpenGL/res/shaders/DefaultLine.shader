#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Trace;

void main()
{
	gl_Position = u_Proj * u_Trace * u_View * u_Model * position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_LineColor;

void main()
{
	color = u_LineColor;
}