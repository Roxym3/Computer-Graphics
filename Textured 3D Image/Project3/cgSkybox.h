#pragma once
#include "cgPoint3D.h"
#include <vector>
#include <GL\glew.h>
#include <GL/glut.h>

using namespace std;

class cgSkybox
{
public:
    cgSkybox(void);
    ~cgSkybox(void);

    void InitData(float sideLength); // ��ʼ������������
    void Render();                   // ��Ⱦ������
    void SetPos(cgPoint3D pos);      // ����������λ��
    void SetTexture(GLuint texture); // ��������
    void LoadCubeMapTexture(const std::vector<std::string>& faces); // ������������ͼ
    void LoadShader(const char* vsFileName, const char* fsFileName); // ������ɫ��

    cgProgram prog;

    unsigned int vboHandle[2]; // ���㻺��������
    unsigned int vaoHandle;    // �����������

    mat4 projectionMat;
    mat4 viewMat;
    mat4 model;
    vec3 color;

private:
    float sideLength;  // ������߳�
    cgPoint3D ptPos;   // ������λ��
    GLuint textureID;  // ����ID
};