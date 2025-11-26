#version 430

#define GRAYSCALE	(1 << 0)
#define COLORTINT	(1 << 1)
#define SCANLINE	(1 << 2)

in vec2 v_texcoord;

out vec4 f_color;

// post process controls
uniform uint u_parameters = 0;
uniform vec3 u_colorTint = vec3(1, 1, 1);

uniform float u_time = 0;
uniform float u_intensity = 1.0;

uniform sampler2D u_baseMap;

void main()
{
	vec4 color = texture(u_baseMap, v_texcoord);

	vec4 postprocess = color;

	postprocess = postprocess * vec4(u_colorTint, 1);

	// apply postprocess effects
	//if((u_parameters & GRAYSCALE) != 0) apply grayscale
	/*
	& (bit1 && bit2) = 1
	| (bit1 || bit2) = 1
	^ (bit1 != bit2) = 1
	~ (!bit1) = 0
	*/
	
	f_color = postprocess;
}