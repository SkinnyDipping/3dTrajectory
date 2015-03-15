#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec4 a_position;
attribute vec2 a_textureCoord;

uniform mat4 u_rotation;
uniform mat4 mvp_matrix;
uniform vec4 u_rotoid;

varying vec2 v_textureCoord;

void main()
{
	vec4 position = u_rotation * (a_position - u_rotoid);
	position = position + u_rotoid;
	gl_Position = mvp_matrix * position;
	v_textureCoord = a_textureCoord;
}
