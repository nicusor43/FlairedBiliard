#version 330 core

in vec3 ex_Color;
in vec2 tex_Coord;

out vec4 out_Color;

uniform sampler2D myTexture;

void main(void)
{
    vec4 vertexColor = vec4(ex_Color, 1.0); 

    out_Color = texture(myTexture, tex_Coord);	
}   