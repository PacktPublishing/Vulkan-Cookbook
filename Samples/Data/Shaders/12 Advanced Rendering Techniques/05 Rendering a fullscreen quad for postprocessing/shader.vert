#version 450

layout( location = 0 ) in vec4 app_position;

void main() {
  gl_Position = app_position;
}
