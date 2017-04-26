#version 450

layout( vertices = 3 ) out;

void main() {
  if( 0 == gl_InvocationID ) {
    gl_TessLevelInner[0] = 3.0;
    gl_TessLevelOuter[0] = 3.0;
    gl_TessLevelOuter[1] = 4.0;
    gl_TessLevelOuter[2] = 5.0;
  }
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
