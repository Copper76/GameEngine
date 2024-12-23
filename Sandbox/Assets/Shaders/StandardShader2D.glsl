#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjectionMatrix;

out vec2 v_TexCoord;
out vec4 v_Colour;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Colour = a_Colour;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
in vec4 v_Colour;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	colour = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Colour;
	//colour = v_Colour;
}