#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 u_rotation;
uniform mat4 mvp_matrix;
uniform vec4 u_centroid;

attribute vec4 a_position;

void main()
{
   vec4 position = u_rotation * (a_position - u_centroid);
   position = position + u_centroid;
   gl_Position = mvp_matrix * position;
}
