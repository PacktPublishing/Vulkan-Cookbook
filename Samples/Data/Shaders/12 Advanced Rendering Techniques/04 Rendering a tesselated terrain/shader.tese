#version 450

layout( triangles, fractional_even_spacing, cw ) in;
layout( location = 0 ) in vec2 tesc_texcoord[];

layout( set = 0, binding = 1 ) uniform sampler2D HeightMap;

layout( location = 0 ) out float tese_height;

void main() {
vec4 position = gl_in[0].gl_Position * gl_TessCoord.x +
                gl_in[1].gl_Position * gl_TessCoord.y +
                gl_in[2].gl_Position * gl_TessCoord.z;

vec2 texcoord = tesc_texcoord[0] * gl_TessCoord.x +
                tesc_texcoord[1] * gl_TessCoord.y +
                tesc_texcoord[2] * gl_TessCoord.z;

  float height = texture( HeightMap, texcoord ).x;
  position.y += height;

  gl_Position = position;
  tese_height = height;
}
