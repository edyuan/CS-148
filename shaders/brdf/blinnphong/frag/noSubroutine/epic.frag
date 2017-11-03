#version 330

in vec4 fragmentColor;
in vec4 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 finalColor;

uniform InputMaterial {
    float matDiffuse;
    float matSpecular;
    float matShininess;
} material;

struct EpicLightProperties {
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 LightColor;
};

float clamp(vec4 in1,vec4 in2) {
    float output1= dot(in1,in2);
    if (output1 <= 0)
	return 0;
    else if (output1 > 1)
        return 1;
    else
        return output1;
}

float clamponly(float output1) {
    if (output1 <= 0)
	return 0;
    else if (output1 > 1)
        return 1;
    else
        return output1;
}

uniform EpicLightProperties genericLight;

struct PointLight {
    vec4 pointPosition;
};

struct DirectionalLight {
    vec4 direction;
};

struct HemisphereLight {
    vec4 colorground;
    vec4 colorsky;
};

uniform PointLight pointLight;
uniform DirectionalLight directionalLight;
uniform HemisphereLight hemisphereLight;

uniform vec4 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform int lightingType;

vec4 hemisphereLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{

    float PI = 3.141592653589793239f;
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = N;

    // Calculate c_diff, c_spec vectors
    vec4 c_diff = fragmentColor * (1-material.matShininess);

    //final definitions of d and s
    vec4 d = c_diff/PI;
    
    //term1
    vec4 upwardnorm = vec4(0.f,1.f,0.f,0.f);
    float term1 = clamponly(0.5*clamp(N,upwardnorm)+0.5);

    // c_light
    vec4 c_light = mix(hemisphereLight.colorground,hemisphereLight.colorsky,term1);
    
    //Color
    vec4 red = vec4(1.f,0.f,0.f,1.f);

    vec4 c_final = c_light*clamp(N,L)*(d); 
    return c_final;
}

vec4 directionalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{

    float PI = 3.141592653589793239f;
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = -1 * directionalLight.direction;

    // Direction from the surface to the eye
    vec4 V = normalize(cameraPosition - worldPosition);

    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + V);

    // Dot Products
    float VdotH = clamp(V,H);

    // Calculate c_diff, c_spec vectors
    vec4 c_diff = fragmentColor * (1-material.matShininess);
    vec4 c_spec = mix(vec4(0.08f *material.matSpecular), fragmentColor, material.matShininess);

    // Amount of specular reflection
    float alpha = material.matDiffuse * material.matDiffuse; 
    float D = alpha*alpha / PI/(clamp(N,H)*clamp(N,H) *(alpha*alpha -1) + 1)/ (clamp(N,H)*clamp(N,H) *(alpha*alpha -1) + 1); 
    float k = pow(material.matDiffuse + 1,2) / 8;
    float G_L = clamp(N,L)/( clamp(N,L)*(1-k)+ k);
    float G_V = clamp(N,V)/( clamp(N,V)*(1-k)+ k);
    vec4 F = c_spec + (1-c_spec)*pow(2,((-5.55473*VdotH-6.98316)*VdotH));

    //final definitions of d and s
    vec4 s = D*F*G_L*G_V/4/clamp(N,L)/clamp(N,V);
    vec4 d = c_diff/PI;

    vec4 c_final = genericLight.LightColor*clamp(N,L)*(d+s); 

    return c_final;
}

vec4 pointLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{

    float PI = 3.141592653589793239f;
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = normalize(pointLight.pointPosition - worldPosition);

    // Direction from the surface to the eye
    vec4 V = normalize(cameraPosition - worldPosition);

    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + V);

    // Dot Products
    float VdotH = clamp(V,H);

    // Calculate c_diff, c_spec vectors
    vec4 c_diff = fragmentColor * (1-material.matShininess);
    vec4 c_spec = mix(vec4(0.08f *material.matSpecular), fragmentColor, material.matShininess);

    // Amount of specular reflection
    float alpha = material.matDiffuse * material.matDiffuse; 
    float D = alpha*alpha / PI/(clamp(N,H)*clamp(N,H) *(alpha*alpha -1) + 1)/ (clamp(N,H)*clamp(N,H) *(alpha*alpha -1) + 1); 
    float k = pow(material.matDiffuse + 1,2) / 8;
    float G_L = clamp(N,L)/( clamp(N,L)*(1-k)+ k);
    float G_V = clamp(N,V)/( clamp(N,V)*(1-k)+ k);
    vec4 F = c_spec + (1-c_spec)*pow(2,((-5.55473*VdotH-6.98316)*VdotH));

    //final definitions of d and s
    vec4 s = D*F*G_L*G_V/4/clamp(N,L)/clamp(N,V);
    vec4 d = c_diff/PI;

    vec4 c_final = genericLight.LightColor*clamp(N,L)*(d+s); 

    return c_final;
}

vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0);
}

vec4 AttenuateLight(vec4 originalColor, vec4 worldPosition)
{
    float lightDistance = length(pointLight.pointPosition - worldPosition);
    float attenuation = 1.0 / (constantAttenuation + lightDistance * linearAttenuation + lightDistance * lightDistance * quadraticAttenuation);
    float l_radius = 100;
    float decrease = 1-(lightDistance/l_radius)*(lightDistance/l_radius)*(lightDistance/l_radius)*(lightDistance/l_radius);
    attenuation = attenuation * clamponly(decrease)*clamponly(decrease);


    return originalColor * attenuation;
}

void main()
{
    vec4 lightingColor = vec4(0);
    if (lightingType == 0) {
        lightingColor = globalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 1) {
        lightingColor = pointLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 2) {
        lightingColor = directionalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 3) {
	lightingColor = hemisphereLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    }

    finalColor = AttenuateLight(lightingColor, vertexWorldPosition);
}
