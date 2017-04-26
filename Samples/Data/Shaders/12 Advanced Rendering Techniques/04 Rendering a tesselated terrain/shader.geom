#version 450

layout( triangles ) in;
layout( location = 0 ) in float tese_height[];

layout( set = 0, binding = 0 ) uniform UniformBuffer {
  mat4 ModelViewMatrix;
  mat4 ProjectionMatrix;
};

layout( triangle_strip, max_vertices = 3 ) out;
layout( location = 0 ) out vec3  geom_normal;
layout( location = 1 ) out float geom_height;

void main() {
  vec3 v0v1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  vec3 v0v2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  vec3 normal = normalize( cross( v0v1, v0v2 ) );
  
  for( int vertex = 0; vertex < 3; ++vertex ) {
    gl_Position = ProjectionMatrix * ModelViewMatrix * gl_in[vertex].gl_Position;
    geom_height = tese_height[vertex];
    geom_normal = normal;
    EmitVertex();
  }
  
  EndPrimitive();
}
