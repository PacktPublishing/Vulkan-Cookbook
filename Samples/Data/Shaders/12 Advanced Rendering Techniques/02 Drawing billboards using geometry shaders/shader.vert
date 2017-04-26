#version 450

layout( location = 0 ) in vec4 app_position;

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ModelViewMatrix;
  mat4 ProjectionMatrix;
};

void main() {
  gl_Position = ModelViewMatrix * app_position;
}
