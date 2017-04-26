#version 450

layout( location = 0 ) in vec3 vert_position;
layout( location = 1 ) in vec3 vert_normal;

layout( set = 0, binding = 1 ) uniform samplerCube Cubemap;

layout( push_constant ) uniform LightParameters {
  vec4 Position;
} Camera;

layout( location = 0 ) out vec4 frag_color;

void main() {
  vec3 view_vector = vert_position - Camera.Position.xyz;
  
  float angle = smoothstep( 0.3, 0.7, dot( normalize( -view_vector ), vert_normal ) );
  
  vec3 reflect_vector = reflect( view_vector, vert_normal );
  vec4 reflect_color = texture( Cubemap, reflect_vector );
  
  vec3 refrac_vector = refract( view_vector, vert_normal, 0.3 );
  vec4 refract_color = texture( Cubemap, refrac_vector );
  
  frag_color = mix( reflect_color, refract_color, angle ) * vec4( 1.0, 0.6, 0.2, 1.0 );
}
