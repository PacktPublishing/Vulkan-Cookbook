#version 450

layout( location = 0 ) in float vert_color;

layout( location = 0 ) out vec4 frag_color;

void main() {
  frag_color = vec4( vert_color, vert_color, vert_color, vert_color );
}
