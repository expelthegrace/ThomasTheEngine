#version 330 core

out vec4 color;

in vec2 uv0;
in vec3 normal0;
in vec4 vertexPos;

uniform sampler2D texture0;
uniform int drawTexture;
uniform vec4 color0;

uniform mat4 view;

vec3 lightDirection = vec3 (0, -1, 0);
vec3 lightPosition  = vec3 (0, 1, 5);
vec3 lightAmbient = vec3(0.2, 0.2, 0.2);

uniform float k_diffuse;
uniform float k_specular;
uniform float u_matShininess;


vec3 BlinPhong(vec3 L, vec3 V, vec3 N, vec3 HD, vec3 diffuseColor) {
	//Diffuse = Dirlight · Normalsurface · Colorsurface
	float diffuseTerm = clamp(dot(N, L), 0, 1) ;
	vec3 diffuse =  vec3(k_diffuse) * diffuseTerm;

	//Specular = (Half · Normalsurface)^shininess
	float specularTerm = 0;

	float sp  = max(dot(V, HD), 0.0);
	if(sp > 0)
	{
		specularTerm = pow(sp, u_matShininess);
	}
	vec3 specular =  vec3(k_specular) * specularTerm;

	//Ambient
	vec3 ambient =  lightAmbient * 1;

	return diffuseColor * (diffuse + specular + ambient);
	//return vec3 (1,1,1);
}

void main()
{
	vec3 view_pos    = transpose(mat3(view))*(-view[3].xyz);

	vec3 L = normalize(lightPosition - vertexPos.xyz);
	vec3 V = normalize(view_pos - vertexPos.xyz);
	vec3 N = normalize(normal0);
	vec3 halfwayDir = normalize(L + V);
	vec3 diffuseColor = (texture2D(texture0, uv0)).xyz;

	vec3 colorBlinPhong = BlinPhong(L,V,N,halfwayDir,diffuseColor);

	if (drawTexture == 1) color = vec4(colorBlinPhong,1.0);
	//if (drawTexture == 1) color = vec4 (normal0.x, normal0.y, normal0.z, 1.0);// color = texture2D(texture0, uv0);
	else color = color0;
}



