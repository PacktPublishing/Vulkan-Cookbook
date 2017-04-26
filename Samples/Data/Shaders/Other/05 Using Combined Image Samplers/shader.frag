#version 450

layout( location = 0 ) in vec2 vert_texcoord;

layout( set = 0, binding = 0 ) uniform sampler2D ImageSampler;

layout( location = 0 ) out vec4 frag_color;

void main() {
  frag_color = texture( ImageSampler, vert_texcoord );
}
