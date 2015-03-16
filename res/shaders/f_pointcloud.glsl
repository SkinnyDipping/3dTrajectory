#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform vec4 u_pointsColor;
varying vec2 v_texcoord;

void main()
{
    gl_FragColor = u_pointsColor;
}

