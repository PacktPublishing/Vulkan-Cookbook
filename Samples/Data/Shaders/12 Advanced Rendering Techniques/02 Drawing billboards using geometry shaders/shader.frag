#version 450

layout( location = 0 ) in vec2 geom_texcoord;

layout( location = 0 ) out vec4 frag_color;

void main() {
  float alpha = 1.0 - dot( geom_texcoord, geom_texcoord );
  if( 0.2 > alpha ) {
    discard;
  }
  frag_color = vec4( alpha );
}
