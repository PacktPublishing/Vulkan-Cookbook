#version 450

layout( location = 0 ) in vec4 app_position;
layout( location = 1 ) in vec2 app_texcoord;

layout( location = 0 ) out vec2 vert_texcoord;

void main() {
  gl_Position = app_position;
  vert_texcoord = app_texcoord;
}
