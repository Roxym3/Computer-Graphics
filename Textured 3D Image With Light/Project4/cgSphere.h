#pragma once
#include "cgPoint3D.h"
#include "cgTriangle.h"
#include<vector>
#include <GL\glew.h>
#include <GL/glut.h>

using namespace std;

extern vec3 lightPos;
extern vec3 lightcolor;
extern vec3 viewPos;

class cgSphere
{
public:
	cgSphere(void);
	~cgSphere(void);

	void InitData(float r);
	void Render();
	void SetPos(cgPoint3D pos);
	void SetTexture(GLuint texture);

	void LoadShader(const char *vsfileName, const char *fsfileName);

	cgProgram prog;
	cgProgram textprog;
	cgProgram lightprog;

	unsigned int vboHandle[3];
	unsigned int vaoHandle;

	mat4 projectionMat;
	mat4 viewMat;
	mat4 model;
	vec3 color;

	vec3 Ka;//������
	vec3 Kd;//�������
	vec3 Ks;//���淴���

	int shiness;//�߹�ϵ��

private:
	float fR;
	cgPoint3D ptPos; 
	vector<cgTriangle> vTris;
	GLuint textureID;

	int iCount;//�����θ���
};

