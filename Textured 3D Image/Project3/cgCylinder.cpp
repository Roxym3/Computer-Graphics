#include "cgProgram.h" 
#include "cgCylinder.h"
#include <math.h>
#include <GL/glew.h>//OpenGL��
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
	glDeleteBuffers(3, vboHandle);//�ͷ��Դ滺����
	glDeleteVertexArrays(1, &vaoHandle);//ɾ�������������
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

	int m = 50;//ˮƽ����Բ��ɢ������α���

	iCount = m * 4;
	//���嶥������
	float* vertc = new float[3 * 3 * iCount];
	float* normal = new float[3 * 3 * iCount];//��������
	float* textcor = new float[3 * 2 * iCount];//��������

	float dalpha = 2 * PI / m;

	int vindex = 0;
	int nindex = 0;
	int tindex = 0;
	vec3 vnormal;

	for (int i = 0; i < m; i++) {
		// ��ǰ�Ƕ�
		float alpha = i * dalpha;
		float alpha_next = (i + 1) * dalpha;

		// ������ϵ� 4 �����㣺�ײ����������������ڵ�Բ���ϵĵ�
		float x1 = r * cos(alpha);
		float y1 = r * sin(alpha);
		float x2 = r * cos(alpha_next);
		float y2 = r * sin(alpha_next);

		// ������ϵĶ���
		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = -fheight;
		vertc[vindex++] = x2; vertc[vindex++] = y2; vertc[vindex++] = -fheight;
		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = fheight;

		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = fheight;
		vertc[vindex++] = x2; vertc[vindex++] = y2; vertc[vindex++] = -fheight;
		vertc[vindex++] = x2; vertc[vindex++] = y2; vertc[vindex++] = fheight;

		// ������
		vnormal = glm::normalize(vec3(x1, y1, 0));  // �������� XY ƽ����
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;

		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;
		normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = 0.0f;

		// ��������
		textcor[tindex++] = i / (float)m; textcor[tindex++] = 0.0f;  // �ײ�
		textcor[tindex++] = (i + 1) / (float)m; textcor[tindex++] = 0.0f;
		textcor[tindex++] = i / (float)m; textcor[tindex++] = 1.0f;  // ����

		textcor[tindex++] = i / (float)m; textcor[tindex++] = 1.0f;  // ����
		textcor[tindex++] = (i + 1) / (float)m; textcor[tindex++] = 0.0f;
		textcor[tindex++] = (i + 1) / (float)m; textcor[tindex++] = 1.0f;  // �ײ�
	}

	for (int i = 0; i < m; i++) {
		float alpha = i * dalpha;

		// ����Բ�Ķ���
		float x1 = fR * cos(alpha);
		float y1 = fR * sin(alpha);
		float z1 = fheight; // ����Բλ���ϰ벿�ֵĸ߶�

		// �ײ�Բ�Ķ���
		float x2 = fR * cos(alpha);
		float y2 = fR * sin(alpha);
		float z2 = -fheight; // �ײ�Բλ���°벿�ֵĸ߶�

		// ��������
		float s1 = (x1 + fR) / (2 * fR); // ��Բ����������ӳ�䵽[0, 1]
		float t1 = (y1 + fR) / (2 * fR);
		float s2 = (x2 + fR) / (2 * fR);
		float t2 = (y2 + fR) / (2 * fR);

		// ����Բ��������
		vertc[vindex++] = 0.0f; vertc[vindex++] = 0.0f; vertc[vindex++] = z1; // Բ��
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = 1.0f; // ����
		textcor[tindex++] = 0.5f; textcor[tindex++] = 0.5f; // ��������

		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = z1; // �ⲿ��
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = 1.0f;
		textcor[tindex++] = s1; textcor[tindex++] = t1;

		vertc[vindex++] = fR * cos(alpha + dalpha); vertc[vindex++] = fR * sin(alpha + dalpha); vertc[vindex++] = z1; // ��һ�ⲿ��
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = 1.0f;
		textcor[tindex++] = (cos(alpha + dalpha) + 1.0f) / 2.0f; textcor[tindex++] = (sin(alpha + dalpha) + 1.0f) / 2.0f;

		// �ײ�Բ��������
		vertc[vindex++] = 0.0f; vertc[vindex++] = 0.0f; vertc[vindex++] = z2; // Բ��
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = -1.0f; // ����
		textcor[tindex++] = 0.5f; textcor[tindex++] = 0.5f; // ��������

		vertc[vindex++] = fR * cos(alpha + dalpha); vertc[vindex++] = fR * sin(alpha + dalpha); vertc[vindex++] = z2;
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = -1.0f;
		textcor[tindex++] = (cos(alpha + dalpha) + 1.0f) / 2.0f; textcor[tindex++] = (sin(alpha + dalpha) + 1.0f) / 2.0f;

		vertc[vindex++] = x1; vertc[vindex++] = y1; vertc[vindex++] = z2;
		normal[nindex++] = 0.0f; normal[nindex++] = 0.0f; normal[nindex++] = -1.0f;
		textcor[tindex++] = s1; textcor[tindex++] = t1;
	}
	glGenBuffers(3, vboHandle);//���ɻ�����������

	glGenVertexArrays(1, &vaoHandle);//���ɶ������飬��vertex shader��
	glBindVertexArray(vaoHandle);//���Զ���������飬ʹ��ʱ��Ҫ������һ��

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//����󶨣���ǰ������
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * iCount * sizeof(float), vertc, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��������������ԣ����0�����꣩��Ӧ
	glEnableVertexAttribArray(0);  // ����Vertex��ɫ�����������0��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//���������ݣ������Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * iCount * sizeof(float), textcor, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ����������루���1���������Զ�Ӧ
	glEnableVertexAttribArray(1);  //  // ����Vertex��ɫ�����������1��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);//���������ݣ����������Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * iCount * sizeof(float), normal, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��㷨�������루���2�������������Զ�Ӧ
	glEnableVertexAttribArray(2);  //  // ����Vertex��ɫ�����������2��ȡ�Դ����ݡ�

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
	//������Ҫ��shader�����Զ�̬�л���ͬ��shader

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

	//�ر����õ�shader
	prog.Unuse();

}
