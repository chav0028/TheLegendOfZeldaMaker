#ifdef GL_ES
in mediump vec2 v_textureCoordinates;
in lowp vec4 v_textureColor;
out lowp vec4 fragColor;
#else
in vec2 v_textureCoordinates;
in vec4 v_textureColor;
out vec4 fragColor;
#endif

uniform sampler2D u_texture;

void main()
{
    fragColor = texture(u_texture, v_textureCoordinates) * v_textureColor;
}