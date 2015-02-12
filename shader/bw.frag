#version 150

uniform sampler2D image;

in vec2 texpos;

void main()
{
    vec4 color = texture(image, texpos);
    gl_FragColor = vec4(vec3((color.x + color.y + color.z) / 3.0), 1.0);
}

