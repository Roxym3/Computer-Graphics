#version 410 //版本号
//输入
layout (location = 0) in vec3 VertexPosition;

//输出
out vec3 Color;

//uniform 变量
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

uniform vec3 objectColor;

void main()
{
    Color = objectColor;
    mat4 mvp=ProjectionMatrix * ViewMatrix * ModelMatrix;
    gl_Position = mvp * vec4(VertexPosition,1.0);
}
