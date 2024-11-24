#version 430 //∞Ê±æ∫≈
// ‰»Î
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;

// ‰≥ˆ
out vec2 TexCoord;
//
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    TexCoord = VertexTexCoord;
    gl_Position = ProjectionMatrix *ViewMatrix*ModelMatrix*        
                  vec4(VertexPosition,1.0);
}
