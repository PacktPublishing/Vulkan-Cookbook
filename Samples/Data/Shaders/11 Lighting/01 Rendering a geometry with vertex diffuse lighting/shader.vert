#version 450

layout( location = 0 ) in vec4 app_position;
layout( location = 1 ) in vec3 app_normal;

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ModelViewMatrix;
  mat4 ProjectionMatrix;
};

layout( location = 0 ) out float vert_color;

void main() {
  gl_Position = ProjectionMatrix * ModelViewMatrix * app_position;
  vec3 normal = mat3( ModelViewMatrix ) * app_normal;
  
  vert_color = max( 0.0, dot( normal, vec3( 0.58, 0.58, 0.58 ) ) ) + 0.1;
}