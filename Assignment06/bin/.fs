#version 300 es

precision mediump float;

in vec2 UV;

out vec4 fragmentColor;

uniform sampler2D sampler;

void main()
{
	fragmentColor = texture(sampler, UV);
}
