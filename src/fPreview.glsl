#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
uniform sampler2D texture;
varying vec2 v_textCoord;

void main() {
	gl_FragColor = texture2D(texture, v_textCoord);
}
