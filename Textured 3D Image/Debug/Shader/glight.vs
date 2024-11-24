#version 410 //版本号
//输入
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexture;
layout (location = 2) in vec3 VertexNormal;

//输出
out vec3 Color;

//uniform 变量
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 Ka; // Ambient reflectivity
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ks; // Specular reflectivity
uniform int shiness;//高光系数

void main()
{
    // gouraud shading
    vec3 Position = vec3(ModelMatrix* vec4(VertexPosition, 1.0));
    vec3 Normal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;//计算模型变换后的法向

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
    Color = ambient +  diffuse + specular;//    
// gouraud shading end

    mat4 mvp=ProjectionMatrix * ViewMatrix * ModelMatrix;
    gl_Position = mvp * vec4(VertexPosition,1.0);
}
