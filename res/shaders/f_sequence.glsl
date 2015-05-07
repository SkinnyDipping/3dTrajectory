#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
uniform sampler2D texture;
varying vec2 v_textCoord;

void main() {

	// Due to glTexImage2D texture shall have [0, 1] coordinates
	// therefore scaling is neccessary
	vec2 textCoord = 0.5*(v_textCoord) - 0.5;

	// Horizontal flip...
	textCoord.y *= -1.0;

	gl_FragColor = texture2D(texture, textCoord);
}
