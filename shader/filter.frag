#version 150

uniform sampler2D image;

in vec2 texpos;

void main()
{
    //gl_FragColor = vec4(texpos, 0.0, 1.0);
    gl_FragColor = texture(image, texpos);
}

