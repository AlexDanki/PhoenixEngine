#version 330 core
out vec4 FragColor;
in vec3 Normal;

struct DirectionalLight
{
    vec3 Direction;
    vec3 Color;
    float Intensity;
}; 

uniform DirectionalLight u_sun;

uniform vec3 u_AmbienteColor;
uniform float u_AmbienteIntensity;
uniform sampler2D u_Texture;
uniform vec3 u_MaterialColor;
in vec2 UV;
void main()
{
    vec3 baseColor = texture(u_Texture, UV).rgb * u_MaterialColor;
    vec3 ambiente = u_AmbienteColor * u_AmbienteIntensity;
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_sun.Direction);
    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 finalColor =  baseColor * ((diffuse * u_sun.Color * u_sun.Intensity ) + ambiente );
    FragColor = vec4(finalColor, 1.0);
}