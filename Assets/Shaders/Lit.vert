#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 Normal;
out vec2 UV;
out vec3 WorldPos;
void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
    Normal = vec3(u_Model * vec4(aNormal, 0.0));
    UV = aUV;
    WorldPos = vec3(u_Model * vec4(aPos, 1.0));
}