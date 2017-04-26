#version 450

layout( triangles ) in;
layout( triangle_strip, max_vertices = 9 ) out;

void main() {

  for( int vertex = 0; vertex < 3; ++vertex ) {
    gl_Position = gl_in[vertex].gl_Position + vec4( 0.0, -0.2, 0.0, 0.0 );
    EmitVertex();

    gl_Position = gl_in[vertex].gl_Position + vec4( -0.2, 0.2, 0.0, 0.0 );
    EmitVertex();

    gl_Position = gl_in[vertex].gl_Position + vec4( 0.2, 0.2, 0.0, 0.0 );
    EmitVertex();

    EndPrimitive();
  }
}
