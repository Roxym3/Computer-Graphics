#include "cgProgram.h" 
#include "cgCylinder.h"
#include <math.h>
#include <GL/glew.h>//OpenGL库
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

cgCylinder::cgCylinder(void)
{
	ptPos = cgPoint3D();
	using glm::vec4;
	model = glm::mat4(1.0);
	textureID = 0;
}


cgCylinder::~cgCylinder(void)
{
	glDeleteBuffers(3, vboHandle);//释放显存缓冲区
	glDeleteVertexArrays(1, &vaoHandle);//删除顶点数组对象
}

void cgCylinder::SetPos(cgPoint3D pos)
{
	ptPos = pos;
	glm::vec3 positionVec(ptPos.x, ptPos.y, ptPos.z);
	model *= glm::translate(model, positionVec);
}

void cgCylinder::InitData(float r, float height)
{
	fR = r;
	fheight = height/2.0f;

	int m = 50;//水平方向圆离散化多边形边数

	iCount = m * 4;
	//定义顶点数组
	float* vertc = new float[3 * 3 * iCount];
	float* normal = new float[3 * 3 * iCount];//法向向量
	float* textcor = new float[3 * 2 * iCount];//纹理坐标

	float dalpha = 2 * PI / m;

	int vindex = 0;
	int nindex = 0;
	int tindex = 0;
	vec3 vnormal;

	for (int i = 0; i < m; i++) {
		// 当前角度
		float alpha = i * dalpha;
		float alpha_next = (i + 1) * dalpha;

		// 外表面上的 4 个顶点：底部、顶部、两个相邻的圆周上的点
		float x1 = r * cos(alpha);
		float y1 = r * sin(alpha);
		float x2 = r * cos(alpha_next);
		float y2 = r * sin(alpha_next);

		// 外表面上的顶点
		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = -fheight;
		vertc[vindex++] = x2; vertc[vindex++] = y2; vertc[vindex++] = -fheight;
		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = fheight;

		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = fheight;
		vertc[vindex++] = x2; vertc[vindex++] = y2; vertc[vindex++] = -fheight;
		vertc[vindex++] = x2; vertc[vindex++] = y2; vertc[vindex++] = fheight;

		// 法向量
		vnormal = glm::normalize(vec3(x1, y1, 0));  // 法向量在 XY 平面内
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;

		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;

		// 纹理坐标
		textcor[tindex++] = i / (float)m; textcor[tindex++] = 0.0f;  // 底部
		textcor[tindex++] = (i + 1) / (float)m; textcor[tindex++] = 0.0f;
		textcor[tindex++] = i / (float)m; textcor[tindex++] = 1.0f;  // 顶部

		textcor[tindex++] = i / (float)m; textcor[tindex++] = 1.0f;  // 顶部
		textcor[tindex++] = (i + 1) / (float)m; textcor[tindex++] = 0.0f;
		textcor[tindex++] = (i + 1) / (float)m; textcor[tindex++] = 1.0f;  // 底部
	}

	for (int i = 0; i < m; i++) {
		float alpha = i * dalpha;

		// 顶部圆的顶点
		float x1 = fR * cos(alpha);
		float y1 = fR * sin(alpha);
		float z1 = fheight; // 顶部圆位于上半部分的高度

		// 底部圆的顶点
		float x2 = fR * cos(alpha);
		float y2 = fR * sin(alpha);
		float z2 = -fheight; // 底部圆位于下半部分的高度

		// 纹理坐标
		float s1 = (x1 + fR) / (2 * fR); // 将圆形纹理坐标映射到[0, 1]
		float t1 = (y1 + fR) / (2 * fR);
		float s2 = (x2 + fR) / (2 * fR);
		float t2 = (y2 + fR) / (2 * fR);

		// 顶部圆的三角形
		vertc[vindex++] = 0.0f; vertc[vindex++] = 0.0f; vertc[vindex++] = z1; // 圆心
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = 1.0f; // 法线
		textcor[tindex++] = 0.5f; textcor[tindex++] = 0.5f; // 纹理中心

		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = z1; // 外部点
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = 1.0f;
		textcor[tindex++] = s1; textcor[tindex++] = t1;

		vertc[vindex++] = fR * cos(alpha + dalpha); vertc[vindex++] = fR * sin(alpha + dalpha); vertc[vindex++] = z1; // 下一外部点
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = 1.0f;
		textcor[tindex++] = (cos(alpha + dalpha) + 1.0f) / 2.0f; textcor[tindex++] = (sin(alpha + dalpha) + 1.0f) / 2.0f;

		// 底部圆的三角形
		vertc[vindex++] = 0.0f; vertc[vindex++] = 0.0f; vertc[vindex++] = z2; // 圆心
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = -1.0f; // 法线
		textcor[tindex++] = 0.5f; textcor[tindex++] = 0.5f; // 纹理中心

		vertc[vindex++] = fR * cos(alpha + dalpha); vertc[vindex++] = fR * sin(alpha + dalpha); vertc[vindex++] = z2;
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = -1.0f;
		textcor[tindex++] = (cos(alpha + dalpha) + 1.0f) / 2.0f; textcor[tindex++] = (sin(alpha + dalpha) + 1.0f) / 2.0f;

		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = z2;
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = -1.0f;
		textcor[tindex++] = s1; textcor[tindex++] = t1;
	}
	glGenBuffers(3, vboHandle);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * iCount * sizeof(float), vertc, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//缓冲区数据（纹理）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * iCount * sizeof(float), textcor, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点纹理输入（序号1，纹理）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);//缓冲区数据（法向量）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * iCount * sizeof(float), normal, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点法向量输入（序号2，法向量）属性对应
	glEnableVertexAttribArray(2);  //  // 允许Vertex着色器中输入变量2读取显存数据。

	glBindVertexArray(0);

	delete[]vertc;
	delete[]normal;
	delete[]textcor;
}

void cgCylinder::SetTexture(GLuint texture)
{
	textureID = texture;
}

void cgCylinder::LoadShader(const char* vsfileName, const char* fsfileName)
{
	prog.CompileShader(vsfileName);
	prog.CompileShader(fsfileName);
	prog.Link();

}

void cgCylinder::Render()
{
	//启用需要的shader，可以动态切换不同的shader

	prog.Use();

	prog.SetUniform("ProjectionMatrix", projectionMat);
	prog.SetUniform("ViewMatrix", viewMat);
	prog.SetUniform("ModelMatrix", model);

	color = glm::vec3(1, 0, 0);
	prog.SetUniform("objectColor", color);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3 * iCount);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	//关闭所用的shader
	prog.Unuse();

}
