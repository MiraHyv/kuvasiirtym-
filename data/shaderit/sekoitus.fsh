#version 330 core
in vec2 uv;

out vec3 color;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float maara;

void main(){
   vec3 c0 = texture(texture0, uv).rgb;
   vec3 c1 = texture(texture1, uv).rgb;
   
   color = mix(c0, c1, maara);
   
}
