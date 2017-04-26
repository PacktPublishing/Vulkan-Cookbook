#version 450

layout( location = 0 ) in vec4 app_position;
layout( location = 1 ) in vec3 app_normal;

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ShadowModelViewMatrix;
  mat4 SceneModelViewMatrix;
  mat4 ProjectionMatrix;
};

layout( push_constant ) uniform LightParameters {
  vec4 Position;
} Light;

layout( location = 0 ) out vec3 vert_normal;
layout( location = 1 ) out vec4 vert_texcoords;
layout( location = 2 ) out vec3 vert_light;

const mat4 bias = mat4( 
  0.5, 0.0, 0.0, 0.0,
  0.0, 0.5, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.5, 0.5, 0.0, 1.0 );

void main() {
  gl_Position = ProjectionMatrix * SceneModelViewMatrix * app_position;

  vert_normal = mat3( SceneModelViewMatrix ) * app_normal;
  vert_texcoords = bias * ProjectionMatrix * ShadowModelViewMatrix * app_position;
  vert_light = (SceneModelViewMatrix * vec4( Light.Position.xyz, 0.0 ) ).xyz;
}
