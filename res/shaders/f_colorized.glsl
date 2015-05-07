uniform sampler2D u_texture;
uniform mat4 u_transformationMatrix;
uniform mat4 u_transformationMatrix_inv;
uniform vec4 u_ref_plane;
uniform vec2 u_rgb_resolution;
uniform vec4 u_rotoid;

varying vec4 v_position;

/**
 * @brief Performs rectangle cast on plane
 * @param point Point to be casted
 * @param plane Plane Ax+By+Cz+D=0 [A,B,C,D]
 */
vec4 rectangleCast(vec4 point, vec4 plane)
{
	float cx = point.x;
	float cy = point.y;
	float cz = point.z;
	float t = (-plane.x*cx - plane.y*cy - plane.z*cz - plane.w) /
		  (plane.x*plane.x + plane.y*plane.y + plane.z*plane.z);
	return vec4(cx+plane.x*t, cy+plane.y*t, cz+plane.z*t,1.0);
}

void main()
{
	vec4 textureCoordinate = v_position;
	textureCoordinate = rectangleCast(textureCoordinate, u_ref_plane);
//	textureCoordinate = u_transformationMatrix_inv * textureCoordinate;
	if(textureCoordinate.z > 130.0)
		gl_FragColor = vec4(0.0,0.0,1.0,1.0);
	if(textureCoordinate.z < 132.0)
		gl_FragColor = vec4(1.0,0.0,0.0,1.0);
	else gl_FragColor = vec4(0.0,1.0,0.0,1.0);
	
//	gl_FragColor = texture2D(u_texture, textureCoordinate.xy);
}

