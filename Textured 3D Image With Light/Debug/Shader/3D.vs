#version 410 //�汾��
//����
layout (location = 0) in vec3 VertexPosition;

//���
out vec3 Color;

//uniform ����
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
