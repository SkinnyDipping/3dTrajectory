varying vec3 v_color_pc;
varying vec4 v_position;

void main()
{
	float d = distance(gl_FragCoord.xy, v_position.xy);
	if (d < 10.0)
    gl_FragColor = vec4(v_color_pc/255.0,1.0);
    else
    gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}

