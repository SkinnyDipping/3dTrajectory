#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec2 v_texcoord;

void main()
{
    gl_FragColor = vec4(1.0,1.0,0.0,1.0);
}

