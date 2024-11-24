#include "cgProgram.h" 
#include "cgSkybox.h"
#include <math.h>
#include <iostream>
#include <GL/glew.h>//OpenGL库
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include "SOIL/SOIL.h"
#include "stb_image.h"

cgSkybox::cgSkybox(void)
{
    ptPos = cgPoint3D();
    using glm::vec4;
    model = glm::mat4(1.0);
    textureID = 0;
}

cgSkybox::~cgSkybox(void)
{
    glDeleteBuffers(2, vboHandle);  // 释放显存缓冲区
    glDeleteVertexArrays(1, &vaoHandle); // 删除顶点数组对象
}

void cgSkybox::SetPos(cgPoint3D pos)
{
    ptPos = pos;
}

void cgSkybox::InitData(float sideLength)
{
    this->sideLength = sideLength;
    float half = sideLength / 2.0f;

    // 定义顶点和索引
    float vertices[] = {
        // 顶点坐标
        -half, -half,  half,  // Front face
         half, -half,  half,
         half,  half,  half,
        -half,  half,  half,

        -half, -half, -half,  // Back face
         half, -half, -half,
         half,  half, -half,
        -half,  half, -half,
    };

    unsigned int indices[] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        0, 4, 7, 7, 3, 0,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Top face
        3, 2, 6, 6, 7, 3,
        // Bottom face
        0, 1, 5, 5, 4, 0
    };

    glGenBuffers(2, vboHandle);
    glGenVertexArrays(1, &vaoHandle);

    glBindVertexArray(vaoHandle);

    // 顶点缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 索引缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void cgSkybox::SetTexture(GLuint texture)
{
    textureID = texture;
}

void cgSkybox::LoadCubeMapTexture(const std::vector<std::string>& faces)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        // 使用 stb_image 加载图像
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            // 检查通道数，根据 RGB 或 RGBA 加载纹理
            GLenum format;
            if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            else
            {
                std::cerr << "Unsupported channel format in: " << faces[i] << std::endl;
                stbi_image_free(data);
                continue;
            }

            // 加载纹理到 GL_TEXTURE_CUBE_MAP
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Failed to load cubemap texture at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    // 设置立方体贴图的纹理参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP); // 生成 mipmap

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // 启用无缝贴图
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // 解除绑定
}

void cgSkybox::LoadShader(const char* vsFileName, const char* fsFileName)
{
    prog.CompileShader(vsFileName);
    prog.CompileShader(fsFileName);
    prog.Link();
}

void cgSkybox::Render()
{
    prog.Use();

    mat4 viewWithoutTranslation = mat4(mat3(viewMat));

    prog.SetUniform("ProjectionMatrix", projectionMat);
    prog.SetUniform("ViewMatrix", viewWithoutTranslation);
    prog.SetUniform("ModelMatrix", model);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    prog.Unuse();
}
