#pragma once
#include "cgPoint3D.h"
#include<vector>
#include <GL\glew.h>
#include <GL/glut.h>

using namespace std;

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

private:
	float fR, fheight;
	cgPoint3D ptPos;
	GLuint textureID;
	int iCount;
};