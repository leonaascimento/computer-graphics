#version 400

layout (location = 0) in vec4 position;

out vec4 vColor;

void main() {
  gl_Position = position;

  float red = position.y + 0.5f;
  float green = position.x + 0.5f;
  float blue = (position.x + position.y) * -1;

  vColor = vec4(red, green, blue, 1);
}
