#version 330 core

in vec2 texUV;



layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = vec4(1, 0, 0, 1);
	//fragColor = vec4(texture(color_texture, texUV).rgb,1);
}
