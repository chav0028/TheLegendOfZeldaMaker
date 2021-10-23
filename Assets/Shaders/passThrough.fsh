#ifdef GL_ES
in lowp vec4 destinationColor;
out lowp vec4 fragColor;
#else
in vec4 destinationColor;
out vec4 fragColor;
#endif


void main()
{
  fragColor = destinationColor;
}