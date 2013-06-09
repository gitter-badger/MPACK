#define NONE 			0.0
#define ALPHA_TEST 		1.0
#define ALPHA_BLEND		2.0

#define EPS			1.0e-4;

precision highp float;

uniform sampler2D texture0;

varying vec2 texCoord;
varying vec4 color;
varying float  shadeType;

vec4 outColor;

bool equal(const float a, const float b)
{
	return abs(a-b)<EPS;
}

void main(void)
{
	outColor = texture2D(texture0, texCoord)*color;
	if(equal(shadeType,ALPHA_TEST))
	{
		if (dot(outColor.rgb,vec3(0.35,0.35,0.35))<=0.1)
		{
			discard;
		}
	}
	gl_FragColor=outColor;
}