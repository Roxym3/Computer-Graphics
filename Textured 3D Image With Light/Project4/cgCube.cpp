#include "cgProgram.h" 
#include "cgCube.h"
#include <math.h>
#include <GL/glew.h>//OpenGL��
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

cgCube::cgCube(void)
{
	ptPos = cgPoint3D();
	using glm::vec4;
	model = glm::mat4(1.0);
	textureID = 0;

    color = glm::vec3(1, 1, 0);

    Ka = vec3(0.0, 0.0, 0.3);//������
    Kd = vec3(0.0, 0.0, 0.4);//�������
    Ks = vec3(1.0, 1.0, 1.0);//���淴���

    shiness = 20;//�߹�ϵ��
}


cgCube::~cgCube(void)
{
	glDeleteBuffers(3, vboHandle);//�ͷ��Դ滺����
	glDeleteVertexArrays(1, &vaoHandle);//ɾ�������������
}

void cgCube::SetPos(cgPoint3D pos)
{
	ptPos = pos;
    glm::vec3 positionVec(ptPos.x, ptPos.y, ptPos.z);
    model *= glm::translate(model, positionVec);
}

void cgCube::InitData(float len, float width, float height) {
    // ��ߴ�
    iCount = 6;
    float halfLen = len / 2.0f;
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // �������� (ÿ���� 4 �����㣬ÿ������ 3 �����꣬�� 6 ��)
    float vertc[] = {
        // Front face (+Z)
        -halfLen, -halfHeight,  halfWidth,  // Bottom-left
         halfLen, -halfHeight,  halfWidth,  // Bottom-right
         halfLen,  halfHeight,  halfWidth,  // Top-right
        -halfLen,  halfHeight,  halfWidth,  // Top-left

        // Back face (-Z)
        -halfLen, -halfHeight, -halfWidth,  // Bottom-left
        -halfLen,  halfHeight, -halfWidth,  // Top-left
         halfLen,  halfHeight, -halfWidth,  // Top-right
         halfLen, -halfHeight, -halfWidth,  // Bottom-right

         // Left face (-X)
         -halfLen, -halfHeight, -halfWidth,  // Bottom-left
         -halfLen, -halfHeight,  halfWidth,  // Bottom-right
         -halfLen,  halfHeight,  halfWidth,  // Top-right
         -halfLen,  halfHeight, -halfWidth,  // Top-left

         // Right face (+X)
          halfLen, -halfHeight, -halfWidth,  // Bottom-left
          halfLen,  halfHeight, -halfWidth,  // Top-left
          halfLen,  halfHeight,  halfWidth,  // Top-right
          halfLen, -halfHeight,  halfWidth,  // Bottom-right

          // Top face (+Y)
          -halfLen,  halfHeight, -halfWidth,  // Bottom-left
          -halfLen,  halfHeight,  halfWidth,  // Bottom-right
           halfLen,  halfHeight,  halfWidth,  // Top-right
           halfLen,  halfHeight, -halfWidth,  // Top-left

           // Bottom face (-Y)
           -halfLen, -halfHeight, -halfWidth,  // Bottom-left
            halfLen, -halfHeight, -halfWidth,  // Bottom-right
            halfLen, -halfHeight,  halfWidth,  // Top-right
           -halfLen, -halfHeight,  halfWidth   // Top-left
    };

    // ����������
    float normal[] = {
        // Front face (+Z)
        0.0f, 0.0f, 1.0f,  // All normals point outwards
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Back face (-Z)
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Left face (-X)
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // Right face (+X)
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Top face (+Y)
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Bottom face (-Y)
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    };

    // ������������
    float textcor[] = {
        // Front face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Back face
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        // Left face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Right face
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        // Top face
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        // Bottom face
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    // ���� VAO �� VBO
    glGenBuffers(3, vboHandle);  // ���㡢����������������

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // ���㻺����
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * iCount * sizeof(float), vertc, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(0);

    // �������껺����
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * iCount * sizeof(float), textcor, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(1);

    // ������������
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * iCount * sizeof(float), normal, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}




void cgCube::SetTexture(GLuint texture)
{
	textureID = texture;
}

void cgCube::LoadShader(const char* vsfileName, const char* fsfileName)
{
	prog.CompileShader(vsfileName);
	prog.CompileShader(fsfileName);
	prog.Link();

}

void cgCube::Render()
{
	//������Ҫ��shader�����Զ�̬�л���ͬ��shader

    prog.Use();

    prog.SetUniform("ProjectionMatrix", projectionMat);
    prog.SetUniform("ViewMatrix", viewMat);
    prog.SetUniform("ModelMatrix", model);

    prog.SetUniform("objectColor", color);

    prog.SetUniform("lightColor", lightcolor);
    prog.SetUniform("lightPos", lightPos);
    prog.SetUniform("viewPos", viewPos);

    prog.SetUniform("Ka", Ka);
    prog.SetUniform("Kd", Kd);
    prog.SetUniform("Ks", Ks);
    prog.SetUniform("shiness", shiness);

	glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_QUADS, 0, 4 * iCount);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	//�ر����õ�shader
	prog.Unuse();

}
