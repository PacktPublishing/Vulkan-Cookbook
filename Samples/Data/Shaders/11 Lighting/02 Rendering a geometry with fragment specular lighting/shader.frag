#version 450

layout( location = 0 ) in vec3 vert_position;
layout( location = 1 ) in vec3 vert_normal;

layout( push_constant ) uniform LightParameters {
  vec4 Position;
} Light;

layout( location = 0 ) out vec4 frag_color;

void main() {
  vec3 normal_vector = normalize( vert_normal );
  vec3 light_vector = normalize( Light.Position.xyz - vert_position );
  float diffuse_term = max( 0.0, dot( normal_vector, light_vector ) );
  
  frag_color = vec4( diffuse_term + 0.1 );

  if( diffuse_term > 0.0 ) {
    vec3 view_vector = normalize( vec3( 0.0, 0.0, 0.0 ) - vert_position.xyz );
    vec3 half_vector = normalize( view_vector + light_vector );

    float shinniness = 60.0;
    float specular_term = pow( dot( half_vector, normal_vector ), shinniness );

    frag_color += vec4( specular_term );
  }
}
