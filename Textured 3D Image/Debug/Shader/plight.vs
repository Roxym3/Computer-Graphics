#version 410 //�汾��
//����
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexture;
layout (location = 2) in vec3 VertexNormal;

//���
out vec3 Normal;
out vec3 Position;

//uniform ����
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

void main()
{
    Position = vec3(ModelMatrix* vec4(VertexPosition, 1.0));
    Normal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;//����ģ�ͱ任��ķ���

    mat4 mvp=ProjectionMatrix * ViewMatrix * ModelMatrix;
    gl_Position = mvp * vec4(VertexPosition,1.0);
}
