#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat4 u_rotationMatrix;

//attribute vec4 a_position;
attribute vec3 a_position;

void main()
{
//    gl_Position = mvp_matrix * a_position;
	vec4 position = u_rotationMatrix * vec4(a_position, 1.0);
    gl_Position = mvp_matrix * position;
}
