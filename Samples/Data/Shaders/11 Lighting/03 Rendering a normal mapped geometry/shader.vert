#version 450

layout( location = 0 ) in vec4 app_position;
layout( location = 1 ) in vec3 app_normal;
layout( location = 2 ) in vec2 app_texcoord;
layout( location = 3 ) in vec3 app_tangent;
layout( location = 4 ) in vec3 app_bitangent;

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ModelViewMatrix;
  mat4 ProjectionMatrix;
};

layout( location = 0 ) out vec3 vert_position;
layout( location = 1 ) out vec2 vert_texcoord;
layout( location = 2 ) out vec3 vert_normal;
layout( location = 3 ) out vec3 vert_tangent;
layout( location = 4 ) out vec3 vert_bitangent;

void main() {
  vec4 position = ModelViewMatrix * app_position;
  gl_Position = ProjectionMatrix * position;
  
  vert_position = position.xyz;
  vert_texcoord = app_texcoord;
  vert_normal = mat3( ModelViewMatrix ) * app_normal;
  vert_tangent = mat3( ModelViewMatrix ) * app_tangent;
  vert_bitangent = mat3( ModelViewMatrix ) * app_bitangent;
}
