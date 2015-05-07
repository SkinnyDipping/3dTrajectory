attribute vec4 a_position;

uniform mat4 u_rotation;
uniform mat4 mvp_matrix;
uniform vec4 u_rotoid;

varying vec4 v_position;

void main()
{
	vec4 position = a_position;
	position = u_rotation * (position - u_rotoid);
	position = position + u_rotoid;
	gl_Position = mvp_matrix * position;

	v_position = a_position;
}
