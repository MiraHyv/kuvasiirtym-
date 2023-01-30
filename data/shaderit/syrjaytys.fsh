#version 330 core
in vec2 uv;

out vec3 color;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float maara;

void main(void){

    vec2 o = vec2(-0.5, -0.5);
    
    vec2 syrjahdys = (texture(texture1, uv).rg + o) * maara;
    
    vec3 c0 = texture(texture0, uv + syrjahdys).rgb;
    
    color = c0;
}
