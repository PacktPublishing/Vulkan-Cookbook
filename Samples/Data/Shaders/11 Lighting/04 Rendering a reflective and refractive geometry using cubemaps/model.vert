#version 450

layout( location = 0 ) in vec4 app_position;
layout( location = 1 ) in vec3 app_normal;

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ModelViewMatrix;
  mat4 ProjectionMatrix;
};

layout( location = 0 ) out vec3 vert_position;
layout( location = 1 ) out vec3 vert_normal;

void main() {
  vert_position = app_position.xyz;
  vert_normal = app_normal;
  
  gl_Position = ProjectionMatrix * ModelViewMatrix * app_position;
}
