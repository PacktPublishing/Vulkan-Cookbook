#version 450

layout( location = 0 ) in vec4 app_position;
layout( location = 1 ) in vec4 app_color;

layout( location = 0 ) out vec4 vert_color;

void main() {
  gl_Position = app_position;
  vert_color = app_color;
}
