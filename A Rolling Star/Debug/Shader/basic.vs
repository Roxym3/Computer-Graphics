#version 410 //∞Ê±æ∫≈
// ‰»Î
layout (location = 0) in vec2 VertexPosition;
layout (location = 1) in vec3 VertexColor;

// ‰≥ˆ
out vec3 Color;

//
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;

void main()
{
    Color = VertexColor;
    gl_Position = ProjectionMatrix * ModelMatrix*vec4(VertexPosition,0,1.0);
}
