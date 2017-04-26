#version 450

layout( set = 0, binding = 0 ) uniform sampler2D Image;

layout( location = 0 ) out vec4 frag_color;

void main() {
  vec4 color = vec4( 0.5 );

  color -= texture( Image, gl_FragCoord.xy + vec2( -1.0,  0.0 ) );
  color += texture( Image, gl_FragCoord.xy + vec2(  1.0,  0.0 ) );
  
  color -= texture( Image, gl_FragCoord.xy + vec2(  0.0, -1.0 ) );
  color += texture( Image, gl_FragCoord.xy + vec2(  0.0,  1.0 ) );
  
  frag_color = abs( 0.5 - color );
}
