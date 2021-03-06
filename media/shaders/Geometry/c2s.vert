#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec2 uv;
out vec4 positionView;
uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
uniform vec3 cameraPosition;
out float dist;
out vec3 normalView;

uniform float Radius;

void main(void) {
	uv = in_Uv;
    dist = distance(cameraPosition,in_Vertex.xyz);
    float Blend = max(0, min((dist) / 100.0, 1));

	vec3 sphereVert = normalize(in_Vertex);
	vec3 normal = mix(in_Normal, sphereVert, Blend);
	normalView = NormalMatrix * normal;
	vec3 vert = mix(in_Vertex, sphereVert, Blend);
	gl_Position = MVPMatrix * vec4(vert, 1);
}
