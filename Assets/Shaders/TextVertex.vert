#version 330 core

layout(location=0) in vec4 vertex;

out vec2 TexCoord;


uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * u_Model  * vec4(vertex.xy,0,1);
    TexCoord = vertex.zw;
}

