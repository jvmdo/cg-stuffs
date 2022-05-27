#version 120
attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying vec3 outNormal;
varying vec2 outUV;
varying vec3 worldVertexPosition;
varying vec3 worldNormalDirection;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	worldVertexPosition=vec3(modelMatrix*vec4(vertex,1.0));
	worldNormalDirection=mat3(modelMatrix)*normal;
}
