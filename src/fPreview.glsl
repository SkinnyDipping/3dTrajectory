#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
uniform sampler2D texture;
varying vec2 v_textCoord;

void main() {
//	gl_FragColor[0] = texture2D(texture, v_textCoord)[0];
//	gl_FragColor[1] = texture2D(texture, v_textCoord)[1];
//	gl_FragColor[2] = texture2D(texture, v_textCoord)[2];
//	gl_FragColor[3] = 1.0;
	gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
