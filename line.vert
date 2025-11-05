#version 330 core
layout(location = 0) in vec4 in_Pos;  
uniform mat4 mat;              
uniform vec4 in_Color;
out vec4 ex_Color;
void main()
{
    ex_Color = in_Color;
    gl_Position = mat * in_Pos;   
}