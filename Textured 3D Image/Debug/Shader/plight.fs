#version 410

in vec3 Normal;
in vec3 Position;

out vec4 FragColor;

uniform vec3 Ka; // Ambient reflectivity
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ks; // Specular reflectivity
uniform int shiness;//高光系数

uniform vec3 lightColor;//环境光与光源强度一样，可以单独传
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() 
{
    // phong shading
    // ambient
    vec3 ambient = Ka * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = Kd * diff * lightColor;
    
    // specular
    //  float specularStrength = 1.0; 
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  //反射光向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
    vec3 specular = Ks * spec * lightColor;      
    vec3 Color = ambient +  diffuse + specular;//    
    // phong shading end

    FragColor = vec4(Color, 1.0);
}
