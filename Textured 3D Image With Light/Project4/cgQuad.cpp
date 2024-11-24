#include "cgQuad.h"
#include <GL/glew.h>//OpenGL¿â


cgQuad::cgQuad(void)
{
	for (int i = 0; i < 4; i++)	{
		Pts[i] = cgPoint3D(); 
	}
	ptPos = cgPoint3D(); 
	textureID = 0;
}

cgQuad::~cgQuad(void)
{
}

void cgQuad::SetPos(cgPoint3D pos) {
	ptPos = pos;
}

void cgQuad::SetTexture(GLuint texture) {
	textureID = texture;
}

void cgQuad::SetVertexData(cgPoint3D* v) {
	for (int i = 0; i < 4; ++i) {
		Pts[i] = v[i];
	}
}

void cgQuad::UpdateTextCoord(float* texcor) {
	if (!texcor) return;

	for (int i = 0; i < 4; i++) {
		Pts[i].s = texcor[i * 2];     
		Pts[i].t = texcor[i * 2 + 1]; 
	}
}

void cgQuad::Render() {
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glNormal3f(Pts[i].nX, Pts[i].nY, Pts[i].nZ);
        glTexCoord2f(Pts[i].s, Pts[i].t);
        glVertex3f(Pts[i].x + ptPos.x, Pts[i].y + ptPos.y, Pts[i].z + ptPos.z);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}