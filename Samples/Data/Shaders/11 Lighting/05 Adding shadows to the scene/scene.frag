#version 450

layout( location = 0 ) in vec3 vert_normal;
layout( location = 1 ) in vec4 vert_texcoords;
layout( location = 2 ) in vec3 vert_light;

layout( set = 0, binding = 1 ) uniform sampler2D ShadowMap;

layout( location = 0 ) out vec4 frag_color;

void main() {
  float shadow = 1.0;
  vec4 shadow_coords = vert_texcoords / vert_texcoords.w;
  
  if( texture( ShadowMap, shadow_coords.xy ).r < shadow_coords.z - 0.005 ) {
    shadow = 0.5;
  }

  vec3 normal_vector = normalize( vert_normal );
  vec3 light_vector = normalize( vert_light );
  float diffuse_term = max( 0.0, dot( normal_vector, light_vector ) );

  frag_color = shadow * vec4( diffuse_term ) + 0.1;
}
