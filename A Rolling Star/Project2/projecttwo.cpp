#include "cgProgram.h" //一定要放在glew之前

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

using glm::vec4;

cgProgram prog;

unsigned int vboHandle[2];
unsigned int vaoHandle;

mat4 projection;
mat4 model1;
mat4 model2;

glm::mat4 trans = glm::mat4(1.0f);

float outerRadius = 90.0f;
float innerRadius = 36.0f;

struct Star {
	float X;
	float Y;
	float alpha;
	float scale;
};

Star Star1 = { 90.0f, 90.0f, 0.0f, 1.0f };
Star Star2 = { 450.0f, 260.0f, 18.0f, 1.0f };

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//设置背景色
}

void LoadShader()
{

	//编译与链接Shader
	prog.CompileShader("Shader/basic.vs");
	prog.CompileShader("Shader/basic.fs");
	prog.Link();
}

void DrawStars() {
	const int numVerticesStar = 10;
	float angleStep = glm::radians(36.0f);
	float rotationAngle[2] = { 0.0f,0.0f };

	float StarsCenters[2][2] = {
		{ Star1.X , Star1.Y },
		{ Star2.X , Star2.Y },
	};

	float* StarVerts = new float[(numVerticesStar + 2) * 2 * 2];
	float* StarColor = new float[(numVerticesStar + 2) * 3 * 2];

	for (int starIndex = 0; starIndex < 2; starIndex++) {
		int offsetVerts = starIndex * (numVerticesStar + 2) * 2;
		int offsetColors = starIndex * (numVerticesStar + 2) * 3;

		StarVerts[offsetVerts] = StarsCenters[starIndex][0];
		StarVerts[offsetVerts + 1] = StarsCenters[starIndex][1];
		StarColor[offsetColors] = 1.0f;
		StarColor[offsetColors + 1] = 0.0f;
		StarColor[offsetColors + 2] = 0.0f;

		int countv = offsetVerts + 2;
		int countc = offsetColors + 3;
		for (int i = 0; i < numVerticesStar + 1; i++) {
			float radius = (i % 2 == 0) ? outerRadius : innerRadius;
			float angle = (i % numVerticesStar) * angleStep;

			float rotatedX = radius * cos(angle + rotationAngle[starIndex]);
			float rotatedY = radius * sin(angle + rotationAngle[starIndex]);

			StarVerts[countv++] = StarsCenters[starIndex][0] + rotatedX;
			StarVerts[countv++] = StarsCenters[starIndex][1] + rotatedY;

			StarColor[countc++] = 1.0f;
			StarColor[countc++] = 0.0f;
			StarColor[countc++] = 0.0f;
		}
	}

	glGenBuffers(2, vboHandle);
	glGenVertexArrays(1, &vaoHandle);

	glBindVertexArray(vaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);
	glBufferData(GL_ARRAY_BUFFER, (numVerticesStar + 2) * 2 * 4 * sizeof(float), StarVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);
	glBufferData(GL_ARRAY_BUFFER, (numVerticesStar + 2) * 3 * 4 * sizeof(float), StarColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	delete[] StarVerts;
	delete[] StarColor;
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}

void collisionReact(float deltax, float deltay, float* vx, float* vy)
{
	float rebounce = outerRadius * (1 + Star2.scale);
	float distanceX = deltax - Star2.X;
	float distanceY = deltay - Star2.Y;
	float distance = distanceX * distanceX + distanceY * distanceY;

	if (distance <= rebounce * rebounce) {
		// 计算法向量并归一化
		glm::vec2 normal = glm::normalize(glm::vec2(distanceX, distanceY));

		// 计算入射速度向量
		glm::vec2 velocity(*vx, *vy);

		// 计算反弹速度
		glm::vec2 newVelocity = velocity - 2.0f * glm::dot(velocity, normal) * normal;

		// 更新 vx 和 vy
		*vx = newVelocity.x;
		*vy = newVelocity.y;
	}
}




void boundaryReact(float deltax, float deltay, float* vx, float* vy)
{

	float leftboundary = 90.0f;
	float rightboundary = 90.0f;
	float topboundary = 90.0f;
	float bottomboundary = 90.0f;

	float rightbounce = 800.0 - deltax;
	float leftbounce = deltax;
	float topbounce = 600.0 - deltay;
	float bottombounce = deltay;

	if (rightbounce < rightboundary || leftbounce < leftboundary) {
		*vx = -*vx;
	}
	if (topbounce < rightboundary || bottombounce < leftboundary) {
		*vy = -*vy;
	}
}

void change()
{
	static float deltax = 90.0, deltay = 90.0,
		vx = 1.2f, vy = 1.2f,
		deltascale = 0.005f;
	Star1.alpha += 72.f;
	Star2.alpha += 18.f;
	if (Star2.scale > 1.2f || Star2.scale < 0.5f) {
		deltascale = -deltascale;
	}
	Star2.scale += deltascale;

	model1 = glm::mat4(1.0f);
	model1 *= glm::translate(trans, glm::vec3(deltax, deltay, 0.0));
	model1 *= glm::rotate(trans, glm::radians(Star1.alpha), glm::vec3(0.0, 0.0, 1.0));
	model1 *= glm::translate(trans, glm::vec3(-Star1.X, -Star1.Y, 0.0));

	model2 = glm::mat4(1.0f);
	model2 *= glm::translate(trans, glm::vec3(Star2.X, Star2.Y, 0.0f));
	model2 *= glm::scale(trans, glm::vec3(Star2.scale, Star2.scale, Star2.scale));
	model2 *= glm::translate(trans, glm::vec3(-Star2.X, -Star2.Y, 0.0f));

	boundaryReact(deltax, deltay, &vx, &vy);
	collisionReact(deltax, deltay, &vx, &vy);
	deltax += vx;
	deltay += vy;

	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)//响应键盘
{
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//启用需要的shader，可以动态切换不同的shader
	prog.Use();

	prog.SetUniform("ProjectionMatrix", projection);

	prog.SetUniform("ModelMatrix", model1);


	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
	glBindVertexArray(0);

	prog.SetUniform("ModelMatrix", model2);
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 12);
	glBindVertexArray(0);

	//关闭所用的shader
	prog.Unuse();
	//glFlush();//单缓冲时必须要，说明绘图命令（函数）结束
	glutSwapBuffers();//交换缓冲（双缓冲时使用）
}

int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//双缓冲，单缓冲为GLUT_SINGLE
	glutCreateWindow("Project One");

	glewInit();//OpenGL初始化,现代OpenGL需要
	init();
	DrawStars();

	LoadShader();//加载Shader

	glutDisplayFunc(display);//图形绘制
	glutReshapeFunc(reshape);//窗口大小变化 
	glutKeyboardFunc(keyboard);//键盘交互
	glutIdleFunc(change);//键盘交互
	glutMainLoop();

	glDeleteBuffers(2, vboHandle);//释放显存缓冲区
	glDeleteVertexArrays(1, &vaoHandle);//删除顶点数组对象

	return 0;
}