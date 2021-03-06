#version 330 core

//#define SAMPLES 10 // 10 is good
//const float invSamples = 1.0/10.0;

#define SAMPLES 10 // 10 is good
const float invSamples = -1.38/10.0;

uniform float totStrength = 1.38;
uniform float strength = 0.07;
uniform float offset = 18.0;
uniform float falloff = 0.000002;
uniform float rad = 0.006;

uniform sampler2D color;
uniform sampler2D normal;
uniform sampler2D depth;
uniform sampler2D noise;

in vec2 uv;
out vec4 fragColor;

void main() {
	// these are the random vectors inside a unit sphere
	vec3 pSphere[10] = vec3[](
		vec3(-0.010735935, 0.01647018, 0.0062425877),
		vec3(-0.06533369, 0.3647007, -0.13746321),
		vec3(-0.6539235, -0.016726388, -0.53000957),
		vec3(0.40958285, 0.0052428036, -0.5591124),
		vec3(-0.1465366, 0.09899267, 0.15571679),
		vec3(-0.44122112, -0.5458797, 0.04912532),
		vec3(0.03755566, -0.10961345, -0.33040273),
		vec3(0.019100213, 0.29652783, 0.066237666),
		vec3(0.8765323, 0.011236004, 0.28265962),
		vec3(0.29264435, -0.40794238, 0.15964167));
 
   // grab a normal for reflecting the sample rays later on
   vec3 fres = normalize((texture(noise,uv*offset).xyz*2.0) - vec3(1.0));
 
   vec4 currentPixelSample = texture(normal,uv);
 
   float currentPixelDepth = texture(depth, uv).r;
 
   // current fragment coords in screen space
   vec3 ep = vec3(uv.xy,currentPixelDepth);
   // get the normal of current fragment
   vec3 norm = currentPixelSample.xyz;
 
   float bl = 0.0;
   // adjust the depth
   float radD = rad/currentPixelDepth;
 
   //vec3 ray, se, occNorm;
   float occluderDepth, depthDifference;
   vec4 occluderFragment;
   vec3 ray;
   
   for(int i=0; i<SAMPLES;++i) {
    // get a vector (randomized inside of a sphere with radius 1.0) from a texture and reflect it
    ray = radD*reflect(pSphere[i],fres);
 
    // get the depth of the occluder fragment
    vec2 vuLookup = ep.xy + sign(dot(ray,norm) )*ray.xy;
    occluderFragment = texture(normal,vuLookup);
    // if depthDifference is negative = occluder is behind current fragment
    depthDifference = currentPixelDepth-texture(depth,vuLookup).r;
 
    // calculate the difference between the normals as a weight
 	// the falloff equation, starts at falloff and is kind of 1/x^2 falling
    bl += step(falloff,depthDifference)*(1.0-dot(occluderFragment.xyz,norm))*(1.0-smoothstep(falloff,strength,depthDifference));
   }
 
   // output the result
   fragColor = vec4(1.0+bl*invSamples);
 
}
