#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
attribute vec2 a_position;
attribute vec2 a_textCoord;
varying vec2 v_textCoord;

void main() {
	gl_Position = vec4(a_position, 0.0, 1.0);
	v_textCoord = a_textCoord;
}
