// OpenGLOld.cpp : 定义控制台应用程序的入口点。
//
#include "cgProgram.h" //一定要放在glew之前
#include <iostream>
#include <Windows.h>

#include <GL/glew.h>//OpenGL库
#include <GL/glut.h>//OpenGL辅助库
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <math.h>
#include "SOIL/SOIL.h"

#include "cgSphere.h"
#include "cgCube.h"
#include "cgSkybox.h"
#include "cgCylinder.h"

#include "Bitmap.h"


#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //这句是不让控制台窗体出现，如果想要出现，去掉即可。

vec3 headdir;
vec3 rightdir;

float step = 0.10f;

float beta = 0.0f;
float alpha = 0.0f;
float anglestep = 20.0f;
float rotationAngle;

float radius;

vec3 pos = vec3(0.0f, 1.0f, 10.0f);
vec3 at = glm::vec3(0.0f, 1.0f, 0.0f);
vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::quat rotation;

cgSphere sphere;
cgCube cube;
cgSkybox skybox;
cgCylinder cylinder;

GLfloat light_position[] = { 0.0, 2.0, -1.0, 0.1 };

int rendermode = 0;//0:填充； 1:线框；2：纹理

GLuint textureID;
int  frames = 0;

std::vector<std::string> faces = {
		"Textures/right.png",
		"Textures/left.png",
		"Textures/top.png",
		"Textures/bottom.png",
		"Textures/front.png",
		"Textures/back.png"
};

GLuint LoadTexture(char* fname)
{
	GLuint tID;

	int width, height, nrComponents;
	unsigned char* data;

	data = SOIL_load_image(fname, &width, &height, &nrComponents, SOIL_LOAD_RGB);// 

	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		bool gammaCorrection = false;
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		//生成纹理
		glEnable(GL_TEXTURE_2D);

		glGenTextures(1, &tID);//生成一个纹理，tID存储纹理索引
		glBindTexture(GL_TEXTURE_2D, tID);

		gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		SOIL_free_image_data(data);

		return tID;

	}

	return -1;
}


void LoadShader()
{
	//编译与链接Shader
//	sphere.LoadShader("Shader/3D.vs", "Shader/3D.fs");
	skybox.LoadShader("Shader/skybox.vs", "Shader/skybox.fs");
	cube.LoadShader("Shader/texture.vs", "Shader/texture.fs");
	cylinder.LoadShader("Shader/texture.vs", "Shader/texture.fs");
	sphere.LoadShader("Shader/texture.vs", "Shader/texture.fs");
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//设置背景色白色
	glEnable(GL_DEPTH_TEST);

	skybox.InitData(10.0f);
	skybox.SetPos(cgPoint3D(pos.x, pos.y, pos.z));

	cube.InitData(2.0f, 2.0f, 2.0f);
	cube.SetPos(cgPoint3D(2.5f, 0.0f, 0.0f));

	cylinder.InitData(1.0f, 3.0f);
	cylinder.SetPos(cgPoint3D(-2.5f, 0.0f, 0.0f));

	sphere.InitData(1.2f);
	sphere.SetPos(cgPoint3D(0.0f, 0.0f, 0.0f));

	LoadShader();

	skybox.LoadCubeMapTexture(faces);
	sphere.SetTexture(LoadTexture("Textures/earth1.jpg"));
	cube.SetTexture(LoadTexture("Textures/pic.png"));
	cylinder.SetTexture(LoadTexture("Textures/water1.bmp"));
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除帧缓存和深度缓存

	//模型观察矩阵初始化
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	headdir = glm::normalize(at - pos);
	rightdir = glm::normalize(glm::cross(up, headdir));

	radius = glm::length(pos - at);

	sphere.viewMat = glm::lookAt(pos, at, up);
	cube.viewMat = glm::lookAt(pos, at, up);
	skybox.viewMat = glm::lookAt(pos, at, up);
	cylinder.viewMat = glm::lookAt(pos, at, up);

	if (rendermode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDisable(GL_DEPTH_TEST);
	skybox.Render();
	glEnable(GL_DEPTH_TEST);
	cube.Render();
	cylinder.Render();
	sphere.Render();

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);//设置视区（窗口内绘图区域）

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	sphere.projectionMat = glm::perspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);
	cube.projectionMat = glm::perspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);
	skybox.projectionMat = glm::perspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);
	cylinder.projectionMat = glm::perspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void change()
{
	//	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'W':   //上移
	case 'w':
		for (int i = 0; i < 3; i++) {
			pos[i] += step * headdir[i];
			at[i] += step * headdir[i];
		}

		break;
	case 'S':   //下移
	case 's':
		for (int i = 0; i < 3; i++) {
			pos[i] -= step * headdir[i];
			at[i] -= step * headdir[i];
		}
		break;
	case 'A':  //左移
	case 'a':
		for (int i = 0; i < 3; i++) {
			pos[i] -= step * rightdir[i];
			at[i] -= step * rightdir[i];
		}
		break;
	case 'D':   //右移
	case 'd':
		for (int i = 0; i < 3; i++) {
			pos[i] += step * rightdir[i];
			at[i] += step * rightdir[i];
		}

		break;

	case 'Z':
	case 'z':
		//抬高相机
		pos[1] += .30f;
		at[1] += .30f;
		break;

	case 'X':
	case 'x':
		//降低相机
		pos[1] -= .30f;
		at[1] -= .30f;
		break;

	case 'T':   //修改绘制模式
	case 't':
		rendermode = (++rendermode) % 2;
		break;
	case '1':
		light_position[0] += 0.1f;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		break;
	case '2':
		light_position[0] -= 0.1f;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		break;
	}

	glutPostRedisplay();
}

void SpecialKey(GLint key, GLint x, GLint y)
{
	rotationAngle = glm::radians(anglestep);

	switch (key)
	{
	case GLUT_KEY_UP: 
		rotation = glm::angleAxis(-rotationAngle, rightdir);

		pos = at + glm::rotate(rotation, pos - at);
		headdir = glm::normalize(at - pos); 
		up = glm::normalize(glm::rotate(rotation, up)); 
		break;

	case GLUT_KEY_DOWN: 
		rotation = glm::angleAxis(rotationAngle, rightdir);

		pos = at + glm::rotate(rotation, pos - at); 
		headdir = glm::normalize(at - pos);       
		up = glm::normalize(glm::rotate(rotation, up)); 
		break;

	case GLUT_KEY_LEFT: 
		rotation = glm::angleAxis(-rotationAngle, up);

		pos = at + glm::rotate(rotation, pos - at); 
		headdir = glm::normalize(at - pos); 
		rightdir = glm::normalize(glm::cross(headdir, up)); 
		break;

	case GLUT_KEY_RIGHT: 
		rotation = glm::angleAxis(rotationAngle, up);

		pos = at + glm::rotate(rotation, pos - at); 
		headdir = glm::normalize(at - pos);        
		rightdir = glm::normalize(glm::cross(headdir, up)); 
		break;
	}

	glutPostRedisplay();
}


int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 200);//应用程序窗口位置
	glutInitWindowSize(800, 600);//窗口大小

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//双缓冲，单缓冲为GLUT_SINGLE，深度缓存
	glutCreateWindow("Project Four");//创建窗口，参数为窗口标题

	glewInit();//OpenGL初始化,现代OpenGL需要
	init();

	glutDisplayFunc(display);//图形绘制
	glutReshapeFunc(reshape);//窗口大小变化
	glutKeyboardFunc(keyboard);//键盘交互
	glutSpecialFunc(&SpecialKey);//方向键
	glutIdleFunc(change);//空闲时间执行

	glutMainLoop();//必须，进入GLUT事件处理循环


	return 0;
}
