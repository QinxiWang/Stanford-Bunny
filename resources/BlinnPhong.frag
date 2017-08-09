#version 330

// Fragment shader

// Textures
uniform sampler2D diffuseRamp;
uniform sampler2D specularRamp;

uniform vec3 eye_world;

// These get passed in from the vertex shader and are interpolated (varying) properties
// change for each pixel across the triangle:
in vec4 interpSurfPosition;
in vec3 interpSurfNormal;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

uniform vec4 lightPosition;
uniform float specularExponent;

uniform vec3 ambientReflectionCoeff;
uniform vec3 diffuseReflectionCoeff;
uniform vec3 specularReflectionCoeff;

uniform vec3 ambientLightIntensity;
uniform vec3 diffuseLightIntensity;
uniform vec3 specularLightIntensity;


void main() {
    
    // Start with black and then add lighting to the final color as we calculate it
    vec3 finalColor = vec3(0.0, 0.0, 0.0);
    
    // direction vectors
    vec3 lightDir = normalize(lightPosition.xyz - interpSurfPosition.xyz);
    vec3 normal = normalize(interpSurfNormal);
    vec3 viewDir = normalize(eye_world - interpSurfPosition.xyz);
    vec3 halfwayVector = normalize(lightDir + viewDir);
    
    // clamping of dot products
    float dotLN = max(dot(lightDir, normal), 0.0);
    float dotHN = max(dot(normal, halfwayVector), 0.0);
    
    // Ambient
    vec3 ambient = ambientReflectionCoeff * ambientLightIntensity;
    
    // Diffuse
    vec4 diffuseTex = texture(diffuseRamp, vec2(dotLN, 0.5));
    vec3 diffuse = diffuseReflectionCoeff * diffuseLightIntensity * diffuseTex.rgb;
    
    // Specular
    vec4 specularTex = texture(specularRamp, vec2(pow(dotHN, specularExponent), 0.5));
    vec3 specular = specularReflectionCoeff * specularLightIntensity * specularTex.rgb;
    
    // TODO: Tell OpenGL to use the r,g,b compenents of materialColor mixed with the reflected color for the color of this fragment (pixel).
    finalColor = ambient + diffuse + specular;
                     
    // Tell OpenGL to use the r,g,b compenents of finalColor for the color of this fragment (pixel).
    fragColor.rgb = finalColor.rgb;
                     
    // And, set the alpha component to 1.0 (completely opaque, no transparency).
    fragColor.a = 1.0;
}
