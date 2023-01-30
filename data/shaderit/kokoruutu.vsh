#version 330 core
out vec2 uv;

const vec4 vertices[6] = vec4[6](
   vec4(-1, -1, 1, 1),
   vec4( 1, -1, 1, 1),
   vec4(-1,  1, 1, 1),

   vec4( 1, -1, 1, 1),
   vec4(-1,  1, 1, 1),
   vec4( 1,  1, 1, 1)
);

const vec2 UVs[6] = vec2[6](
   vec2(0, 0),
   vec2(1, 0),
   vec2(0, 1),

   vec2(1, 0),
   vec2(0, 1),
   vec2(1, 1)
);

void main(void) {
   uv = UVs[gl_VertexID];
   gl_Position = vertices[gl_VertexID];
}
