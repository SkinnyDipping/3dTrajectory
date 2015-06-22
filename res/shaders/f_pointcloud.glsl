varying vec3 v_color_pc;

void main()
{
//    gl_FragColor = vec4(1.0,1.0,0.0,1.0);
    gl_FragColor = vec4(v_color_pc/255.0,1.0);
}

