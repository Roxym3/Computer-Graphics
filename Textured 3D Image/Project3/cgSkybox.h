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

    void InitData(float sideLength); // 初始化立方体数据
    void Render();                   // 渲染立方体
    void SetPos(cgPoint3D pos);      // 设置立方体位置
    void SetTexture(GLuint texture); // 设置纹理
    void LoadCubeMapTexture(const std::vector<std::string>& faces); // 加载立方体贴图
    void LoadShader(const char* vsFileName, const char* fsFileName); // 加载着色器

    cgProgram prog;

    unsigned int vboHandle[2]; // 顶点缓冲区对象
    unsigned int vaoHandle;    // 顶点数组对象

    mat4 projectionMat;
    mat4 viewMat;
    mat4 model;
    vec3 color;

private:
    float sideLength;  // 立方体边长
    cgPoint3D ptPos;   // 立方体位置
    GLuint textureID;  // 纹理ID
};