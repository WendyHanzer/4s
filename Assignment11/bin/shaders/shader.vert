#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 fNormal;
out vec3 fPosition;
out vec2 fUV;
out vec3 oh;

uniform mat4 MVP;	//model view projection matrix
uniform mat4 M;		//model matrix

//N = transpose(inverse(M));
//uniform mat3 N;		//normal matrix

void main()
{
	vec4 vPos = vec4(position, 1);

	gl_Position =  MVP*vPos;
	
	//this should be N*normal, but N
	//is totally broken, always makes the normal
	//perpendicular to the light, giving a dot
	//product of 0... this happens no matter what ...
	fNormal = mat3(M)*normal;
	fPosition = (M*vPos).xyz;
	fUV = uv;
} //main
