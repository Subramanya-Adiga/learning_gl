
  #version 460 core
  layout (location = 0) in vec3 apos;
  layout (location = 1) in vec3 aColor;

  out vec3 oColor;

  void main(){
  gl_Position = vec4(apos,1.0f);
  oColor = aColor;
  }
