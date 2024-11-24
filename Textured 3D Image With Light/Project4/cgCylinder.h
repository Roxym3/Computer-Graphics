#pragma once
#include "cgPoint3D.h"
#include<vector>
#include <GL\glew.h>
#include <GL/glut.h>

using namespace std;

extern vec3 lightPos;
extern vec3 lightcolor;
extern vec3 viewPos;

class cgCylinder
{
public:
	cgCylinder(void);
	~cgCylinder(void);

	void InitData(float r, float height);
	void Render();
	void SetPos(cgPoint3D pos);
	void SetTexture(GLuint texture);

	void LoadShader(const char* vsfileName, const char* fsfileName);

	cgProgram prog;
	cgProgram textprog;

	unsigned int vboHandle[3];
	unsigned int vaoHandle;

	mat4 projectionMat;
	mat4 viewMat;
	mat4 model;
	vec3 color;

	vec3 Ka;//环境光
	vec3 Kd;//漫反射光
	vec3 Ks;//镜面反射光

	int shiness;//高光系数
private:
	float fR, fheight;
	cgPoint3D ptPos;
	GLuint textureID;
	int iCount;
};