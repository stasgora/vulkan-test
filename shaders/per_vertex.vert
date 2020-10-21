#version 450

layout(location = 0) in vec4 vVertex;
layout(location = 3) in vec3 vNormal;

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;

	vec4 Light0Ambient;
	vec4 Light0Diffuse;
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;

	vec3 vLightPosition;
} ubo;

layout(location = 0) smooth out vec4 vVaryingColor;

void main( void ) {
	mat4 mvMatrix = ubo.view * ubo.model;
	vec3 vEyeNormal = mat3(mvMatrix) * vNormal;
	vec4 vPosition4 = mvMatrix * vVertex;

	vec3 vLightDir = normalize( ubo.vLightPosition - vPosition4.xyz / vPosition4.w );

	vec3 ambientColor = ubo.Light0Ambient.rgb * ubo.MaterialAmbient.rgb;

	float diffuseIntensity = max( 0.0, dot( vEyeNormal, vLightDir ) );
	vec3 diffuseColor = diffuseIntensity * ubo.Light0Diffuse.rgb * ubo.MaterialDiffuse.rgb;

	vVaryingColor.rgb =  ambientColor + diffuseColor;
	vVaryingColor.a = ubo.MaterialDiffuse.a;

	gl_Position = ubo.proj * mvMatrix * vVertex;
}
