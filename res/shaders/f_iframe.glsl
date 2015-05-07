#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform vec2 u_rgb_resolution;

varying vec2 v_textureCoord;

void main()
{
    // Set fragment color from texture
    gl_FragColor = texture2D(texture, v_textureCoord/u_rgb_resolution);
}
