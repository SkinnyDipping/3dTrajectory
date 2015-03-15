#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_textureCoord;

void main()
{
	gl_FragColor = vec4(0.0,1.0,0.0,1.0);
//	gl_FragColor = texture2D(texture, v_textureCoord);
}

