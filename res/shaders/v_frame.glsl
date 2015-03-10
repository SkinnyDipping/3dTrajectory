#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec4 a_position;
attribute vec2 a_textureCoord;

// viewing-proper uniforms
uniform mat4 u_rotation;
uniform mat4 mvp_matrix;
uniform vec4 u_rotoid;

// matrix of cast transformation
uniform mat4 u_transformationMatrix;

varying vec2 v_textureCoord;

void main()
{
	vec4 position = u_transformationMatrix * a_position;
	position = u_rotation * (position - u_rotoid);
	position = position + u_rotoid;
	gl_Position = mvp_matrix * position;

	v_textureCoord = a_textureCoord;

}
