#version 430

in vec2 TexCoord;

out vec4 FragColor;

// texture sampler
layout(binding=0) uniform sampler2D texture0;//version 4.2����

void main() 
{
    vec4 tex = texture(texture0, TexCoord);
    FragColor = tex;
}
