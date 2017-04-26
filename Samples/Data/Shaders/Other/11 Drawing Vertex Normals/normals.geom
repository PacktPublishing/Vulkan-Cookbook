#version 450

layout( triangles ) in;

layout( location = 0 ) in vec4 vert_normal[];

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ModelViewMatrix;
  mat4 ProjectionMatrix;
};

layout( line_strip, max_vertices = 6 ) out;

layout( location = 0 ) out vec4 geom_color;

void main() {
  for( int vertex = 0; vertex < 3; ++vertex ) {
    gl_Position = ProjectionMatrix * gl_in[vertex].gl_Position;
    geom_color = vec4( 0.2 );
    EmitVertex();

    gl_Position = ProjectionMatrix * (gl_in[vertex].gl_Position + vert_normal[vertex]);
    geom_color = vec4( 0.6 );
    EmitVertex();

    EndPrimitive();
  }
}
