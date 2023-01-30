#version 330 core
uniform sampler2D texture0;
in vec2 uv;
out vec3 color;

void main(void){
   color = texture(texture0, uv).rgb;
}
