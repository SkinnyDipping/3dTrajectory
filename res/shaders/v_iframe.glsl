#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec4 a_position;

// viewing-proper uniforms
uniform mat4 u_rotation;
uniform mat4 mvp_matrix;
uniform vec4 u_rotoid;

uniform vec2 u_rgb_resolution;

// matrix of cast transformation
uniform mat4 u_transformationMatrix;

varying vec2 v_textureCoord;

void main()
{
	v_textureCoord = vec2(a_position.xy);

	// Laying image on XY plane
	vec4 position = a_position;
	
	// XXX: Is vertical flip needed?

	// Translating image's centroid to origin
	position.x -= u_rgb_resolution.x/2.0;
	position.y -= u_rgb_resolution.y/2.0;

	// Transformating image to cloud
	position = u_transformationMatrix * position;

	// Viewing-proper transformations
	position = u_rotation * (position - u_rotoid);
	position = position + u_rotoid;

	gl_Position = mvp_matrix * position;
	gl_PointSize = 1.0;
}
