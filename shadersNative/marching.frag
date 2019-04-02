#version 430

// #define MAX_LIGHTS = 8;

// Interpolated values from the vertex shaders
in vec3 fColorIn;
//in vec2 tFrag_in;
//in vec3 nFrag_in;

// Ouput data
out vec3 color;

struct Light {
       vec3 position;
       vec3 color;
       vec3 materialSpecularColor;
       float ambientCoeffience;
       float materialShininess;
       float attentuationFactor;
};

//uniform sampler2D diffuseTexture;
uniform sampler3D diffuse3DTexture;
uniform mat4 M;
//uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform mat3 normalMatrix;
uniform Light lights[8];

void main(){
        //color = vec3(0.2f,0.3f,0.9f);
        color = fColorIn;
        //color = vec3(vFrag_in);
////        // Verteksin paikka maailmassa.
////	vec3 vPositionCamera = (M * vec4(vFrag_in,1.0f)).xyz;
////
////	// Normaali. Jos mallia on venytetty ja liikuteltu, niin normaali täyy kertoa normaali matriisilla.
////	//vec3 normal = normalize(nFrag_in);
////	vec3 normal = normalize(normalMatrix * nFrag_in);
////
////	// Valon suunta verteksiin näen.
////	vec3 lightDirection = normalize(lights[0].position - vPositionCamera);
////	
////	// Valon teho.
////	float diffuseCoeffient = max(0.0 , dot(normal,lightDirection));
////	
////	// Valon pävastainen suuntavektori. Tarvitaan reflect funtiota varten.
////	vec3 incidenceVector = -lightDirection;
////
////	// Laskee heijastuneen valon suunnan.
////	vec3 reflectionVector = reflect(incidenceVector, normal);
////
////        // Kameran suunta suhteessa verteksiin.
////        vec3 cameraDirection = normalize(cameraPosition - vPositionCamera);
////
////        // Valon intensiteetti suhteessa kameraan. Tarvitaan siis heijastuksen laskemiseen.
////        float cosAngle = max(0.0, dot(cameraDirection,reflectionVector));
////
////        // Heijastus intensiteetti. Jos pinta on pois pä valon suunnasta, niin täöintensiteetti on 0.0;
////        float specularCoeffient = 0.0;
////        if (diffuseCoeffient > 0.0)
////           specularCoeffient = pow(cosAngle, lights[0].materialShininess);
////
////        // Heijastunvä.
////        vec3 specularComponent = specularCoeffient * lights[0].materialSpecularColor * lights[0].color;
////
////        // Pinnan vä.
////	//vec3 surfaceColor = texture3D(diffuseTexture,vec3(tFrag_in,1.0f)).rgb;
////	vec3 surfaceColor = texture(diffuse3DTexture,vec3(tFrag_in,0.5)).rgb;
//////	vec3 surfaceColor = texture(diffuse3DTexture,vFrag_in.xyz).rgb;
////	//vec3 surfaceColor = vec3(0.5,0.5,0.5);
////
////	// Ambient vä.
////        vec3 ambient = lights[0].ambientCoeffience * lights[0].color * surfaceColor;
////
////        // Pinnan vä ottaen huomioon valon teho ja vä.
////        vec3 diffuseComponent = diffuseCoeffient * lights[0].color * surfaceColor;
////
////        // Etäyys verteksistäaloon.
////        float distanceToLight = distance(vPositionCamera,lights[0].position);
////
////        // Valon vaimennuskerroin.
////        float attentuation = 1.0 / (1.0 + lights[0].attentuationFactor * pow(distanceToLight,2.0));
////
////        // Lopullinen vä.
////        //color = ambient + attentuation * (diffuseComponent + specularComponent);
////        //color = ambient;
////	color = ambient + attentuation * (diffuseComponent + specularComponent);
}