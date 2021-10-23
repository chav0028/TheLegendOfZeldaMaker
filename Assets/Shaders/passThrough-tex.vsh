in vec4 a_vertices;
in vec2 a_textureCoordinates;
in vec4 a_textureColor;

#ifdef GL_ES
out mediump vec2 v_textureCoordinates;
out lowp vec4 v_textureColor;
#else
out vec2 v_textureCoordinates;
out vec4 v_textureColor;
#endif

uniform mat4 u_modelViewProjection;

void main()
{
  v_textureCoordinates = a_textureCoordinates;
  v_textureColor = a_textureColor;
  gl_Position = u_modelViewProjection * a_vertices;
}