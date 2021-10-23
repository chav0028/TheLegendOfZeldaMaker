in vec4 a_vertices;
in vec4 a_sourceColor;
in float a_pointSize;

#ifdef GL_ES
out lowp vec4 destinationColor;
#else
out vec4 destinationColor;
#endif

uniform mat4 u_modelViewProjection;

void main()
{
  destinationColor = a_sourceColor;
  gl_Position = u_modelViewProjection * a_vertices;
  gl_PointSize = a_pointSize;
}
