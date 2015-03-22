#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 u_rotation;
uniform mat4 mvp_matrix;
uniform vec4 u_rotoid;

attribute vec4 a_position;
attribute vec3 a_color;

varying vec3 v_color;

void main()
{
   vec4 position = u_rotation * (a_position - u_rotoid);
   position = position + u_rotoid;
   gl_Position = mvp_matrix * position;
   v_color = a_color;
}
