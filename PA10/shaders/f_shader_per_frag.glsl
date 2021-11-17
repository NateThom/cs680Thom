#version 330

in vec3 fN;
in vec3 objPos;
in vec3 fE;
in vec2 texture;
in float mat_text;

out vec4 fragColor;

struct PositionalLight
{   vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct SpotLight
{   vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct Material
{   vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform int lightCount;
uniform vec4 globalAmbient;
uniform PositionalLight light[1];
uniform SpotLight spot;
uniform Material material;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform sampler2D sampler;
uniform int cutoffAngle;

void main(void)
{
    vec4 textureColor = texture2D(sampler, texture);
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);

    vec4 ambient = globalAmbient * material.ambient;
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    
    vec3 SpotDir = normalize(vec3(0.0,-1.0,0.0));
    
    for(int i=0; i < lightCount; i++){
      vec3 L = normalize(light[i].position - objPos);
      vec3 R = normalize(reflect(-L,N));
      
      ambient += light[i].ambient * material.ambient;
      float Kd = max(dot(L,N),0.0);
      diffuse += Kd * light[i].diffuse * material.diffuse;
      float Ks = pow(max(dot(R,E),0.0), material.shininess);
      specular += Ks * light[i].specular * material.specular;
      if(dot(L,N) < 0.0) specular = vec4(0.0,0.0,0.0,1.0);
    }
    
    vec3 L = normalize(spot.position - objPos);
    vec3 R = normalize(reflect(-L,N));
    
    float SpotAngle = degrees(acos(dot(L,SpotDir)));
    
    if (SpotAngle > cutoffAngle) {
      ambient += spot.ambient * material.ambient;
      float Kd = max(dot(L,N),0.0);
      diffuse += Kd * spot.diffuse * material.diffuse;
      float Ks = pow(max(dot(E,R),0.0), material.shininess);
      specular += Ks * spot.specular * material.specular;
      if(dot(L,N) < 0.0) specular = vec4(0.0,0.0,0.0,1.0);
    
      if(mat_text == 0.0){
        fragColor = (ambient + diffuse) * textureColor + specular;
        fragColor.a = 1.0f;
      }
      else{
        fragColor = ambient + diffuse + specular;
        fragColor.a = 1.0f;
      }
    }
    
    else {
      if(mat_text == 0.0){
        fragColor = (ambient + diffuse) * textureColor + specular;
        fragColor.a = 1.0;
      }
      else {
        fragColor = ambient + diffuse + specular;
        fragColor.a = 1.0;
      }
    }
}
