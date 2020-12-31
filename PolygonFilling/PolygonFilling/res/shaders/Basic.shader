#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//layout(location = 1) in vec2 uv;
//layout(location = 2) in vec4 color;
//out vec4 vertexColor; 
//out vec2 v_uv;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   //vertexColor = color;
   //v_uv = uv;
};

#shader fragment
#version 330 core

out vec4 color;
//in vec4 vertexColor;

//in vec2 v_uv;

uniform vec4 u_Color;
//uniform sampler2D u_Texture;
void main()
{
	//vec4 texColor = texture(u_Texture, v_uv);
	//color = texColor;
	color = u_Color;
};