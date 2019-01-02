#version 330 core

out vec4 color;

in vec2 uv0;
in vec3 normal0;
in vec4 vertexPos;

uniform sampler2D texture0;
uniform int drawTexture;
uniform vec4 color0;

uniform vec3 viewDirection;
uniform vec3 viewPosition;

vec3 lightDirection = vec3 (0, -1, 0);
vec3 lightPosition  = vec3 (0, 1, 5);
vec3 lightAmbient = vec3(0.2, 0.2, 0.2);

vec3 materialDiffuseReflectances = vec3 (1,1,1);
vec3 materialSpecularReflectances = vec3 (1,1,1);
vec3 materialAmbientReflectances = vec3 (1,1,1);
uniform float u_matShininess = 64;

vec3 BlinPhong(vec3 L, vec3 V, vec3 N, vec3 HW, vec3 diffuseColor) {
	//Diffuse = Dirlight · Normalsurface · Colorsurface
	float diffuseTerm = clamp(dot(N, L), 0, 1) ;
	vec3 diffuse =  materialDiffuseReflectances * 1 * diffuseTerm;

	//Specular = (Half · Normalsurface)^shininess
	float specularTerm = 0;

	if(dot(N, L) > 0)
	{
		specularTerm = pow(dot(N, HW), u_matShininess);
	}
	vec3 specular = 1 * materialSpecularReflectances * specularTerm;

	//Ambient
	vec3 ambient =  materialAmbientReflectances * 1;

	return diffuseColor * (diffuse + specular + ambient);
	//return vec3 (1,1,1);
}

void main()
{
	vec3 L = normalize(lightPosition - vertexPos.xyz);
	vec3 V = normalize(viewPosition - vertexPos.xyz);
	vec3 N = normalize(normal0);
	vec3 halfwayDir = normalize(L + V);
	vec3 diffuseColor = (texture2D(texture0, uv0)).xyz;

	vec3 colorBlinPhong = BlinPhong(L,V,N,halfwayDir,diffuseColor);

	if (drawTexture == 1) color = vec4(colorBlinPhong,1.0);
	//if (drawTexture == 1) color = vec4 (normal0.x, normal0.y, normal0.z, 1.0);// color = texture2D(texture0, uv0);
	else color = color0;
}



