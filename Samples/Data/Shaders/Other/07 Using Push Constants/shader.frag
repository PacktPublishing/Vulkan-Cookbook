#version 450

layout( location = 0 ) out vec4 frag_color;

layout( push_constant ) uniform ColorBlock {
  vec4 Color;
} PushConstant;

void main() {
  frag_color = PushConstant.Color;
}
