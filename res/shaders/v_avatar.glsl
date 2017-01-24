#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
uniform mat4 u_rotation;
uniform mat4 mvp_matrix;
uniform vec4 u_rotoid;
uniform vec3 u_trajectory_point;
uniform vec2 u_trajectory_screen;
attribute vec2 a_avatar_corners;
//attribute vec2 a_textCoord;
//varying vec2 v_textCoord;
varying vec2 v_trajectory_screen;

void main() {
	//gl_Position = vec4(a_position, 0.0, 1.0);
	//v_textCoord = a_textCoord;
   vec4 position = u_rotation * (vec4(u_trajectory_point,1.0) - u_rotoid);
   position = position + u_rotoid;
   //gl_Position = mvp_matrix * position;
   vec4 trajectory_pt_screen_coors = mvp_matrix * position;
   vec4 avatar_zero_pt = vec4(a_avatar_corners,0.0,1.0);
   // gl_Position = avatar_zero_pt;
   gl_Position = trajectory_pt_screen_coors;
   // gl_Position = avatar_zero_pt + trajectory_pt_screen_coors;
   v_trajectory_screen=u_trajectory_screen;
}
