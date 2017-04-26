#version 450

layout( location = 0 ) in vec3 vert_position;
layout( location = 1 ) in vec2 vert_texcoord;
layout( location = 2 ) in vec3 vert_normal;
layout( location = 3 ) in vec3 vert_tangent;
layout( location = 4 ) in vec3 vert_bitangent;

layout( set = 0, binding = 1 ) uniform sampler2D ImageSampler;

layout( push_constant ) uniform LightParameters {
  vec4 Position;
} Light;

layout( location = 0 ) out vec4 frag_color;

void main() {
  vec3 normal = 2 * texture( ImageSampler, vert_texcoord ).rgb - 1.0;
  
  vec3 normal_vector = normalize( mat3( vert_tangent, vert_bitangent, vert_normal) * normal );
  vec3 light_vector = normalize( Light.Position.xyz - vert_position );
  float diffuse_term = max( 0.0, dot( normal_vector, light_vector ) ) * max( 0.0, dot( vert_normal, light_vector ) );
  
  frag_color = vec4( diffuse_term + 0.1 );
  
  if( diffuse_term > 0.0 ) {
    vec3 half_vector = normalize(normalize( -vert_position.xyz  ) + light_vector);
    float specular_term = pow( dot( half_vector, normal_vector ), 60.0 );
  
    frag_color += vec4( specular_term );
  }
}
