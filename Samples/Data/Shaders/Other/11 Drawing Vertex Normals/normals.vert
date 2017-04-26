#version 450

layout( location = 0 ) in vec4 app_position;
layout( location = 1 ) in vec3 app_normal;

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ModelViewMatrix;
  mat4 ProjectionMatrix;
};

layout( location = 0 ) out vec4 vert_normal;

void main() {
  gl_Position = ModelViewMatrix * app_position;
  vert_normal = vec4( mat3( ModelViewMatrix ) * app_normal * 0.2, 0.0 );
}
