#version 330 core
out vec4 FragColor;
in vec3 Normal;

struct DirectionalLight
{
    vec3 Direction;
    vec3 Color;
    float Intensity;
}; 

struct Fog
{
    bool Enabled;
    vec3 Color;
    float Density;
};



uniform DirectionalLight u_sun;
uniform Fog u_Fog;

uniform vec3 u_CameraPosition;

uniform vec3 u_AmbienteColor;
uniform float u_AmbienteIntensity;
uniform sampler2D u_Texture;
uniform vec3 u_MaterialColor;

in vec2 UV;
in vec3 WorldPos;
void main()
{
    
    // CorBase
    vec3 baseColor = texture(u_Texture, UV).rgb * u_MaterialColor;




    // AMBIENTE -------------------------------------------------
    // Cor Ambiente
    vec3 ambiente = u_AmbienteColor * u_AmbienteIntensity;



    // DIRECTIONAL LIGHT ----------------------------------------
    // Normalizando valores para calculo do diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_sun.Direction);

    // Calculo do diffuse
    float diffuse = max(dot(normal, lightDir), 0.0);



    // FINAL COLOR -----------------------------------------------
    // Calculo da cor final
    vec3 finalColor =  baseColor * ((diffuse * u_sun.Color * u_sun.Intensity ) + ambiente );



    // FOG -------------------------------------------------------
    // Calculando distancia do fog
    float fogDistance = distance(WorldPos, u_CameraPosition);

    float fogFactor = 1.0 - exp(-u_Fog.Density * fogDistance);  // exp() eleva e( 2,7182) ao que é passado na função
    fogFactor = clamp(fogFactor, 0.0f, 1.0f);                   // 1 - n isso é feito pq o  resultado do exponencial 
                                                                // vai de 1 à 0, subrair 1 por n nos dá a porcentagem
                                                                // do fogFator indo de 0 até 1

    vec3 final = mix(finalColor, u_Fog.Color, fogFactor);

    // END -------------------------------------------------------
    // Aplicando cor final
    //FragColor = vec4(vec3(fogDistance / 20), 1.0);
    FragColor = vec4(final, 1.0);
}