#version 450

layout( location = 0 ) in vec4 app_position;

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ModelViewMatrix;
  mat4 ProjectionMatrix;
};

layout( location = 0 ) out vec3 vert_texcoord;

void main() {
  vec3 position = mat3(ModelViewMatrix) * app_position.xyz;
  gl_Position = (ProjectionMatrix * vec4( position, 0.0 )).xyzz;
  vert_texcoord = app_position.xyz;
}