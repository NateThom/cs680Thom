#version 410

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_texture;

out vec4 ambient;
out vec4 diffuse;
out vec4 specular;
out vec2 texture;
out float mat_text;

struct PositionalLight
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec3 position;
};

struct SpotLight
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec3 position;
};

struct Material
{
  vec4 ambient;
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
  vec4 v = vec4(v_position, 1.0);
  vec3 pos = (viewMatrix * v).xyz;
  vec3 E = normalize(pos);
  vec3 N = normalize(modelMatrix * vec4(v_normal,0.0)).xyz;
  
  vec3 SpotDir = normalize(vec3(0.0,-1.0,0.0));
  
  ambient = globalAmbient * material.ambient;
  diffuse = vec4(0.0, 0.0, 0.0, 0.0);
  specular = vec4(0.0, 0.0, 0.0, 0.0);
  
  for(int i = 0; i < lightCount; i++){
    vec3 L = normalize(light[i].position - (modelMatrix * v).xyz);
    vec3 R = reflect(-L,N);
  
    ambient += light[i].ambient * material.ambient;
    float Kd = max(dot(L,N),0.0);
    diffuse += Kd * light[i].diffuse * material.diffuse;
    float Ks = pow(max(dot(R,E),0.0), material.shininess);
    specular += Ks * light[i].specular * material.specular;
    if(dot(L,N) < 0.0) specular = vec4(0.0,0.0,0.0,1.0);
  }
  
  vec3 L = normalize(spot.position - (modelMatrix * v).xyz);
  vec3 R = reflect(-L,N);
  
  float SpotAngle = degrees(acos(dot(L,SpotDir)));
  
  if (SpotAngle > cutoffAngle) {  
    ambient += spot.ambient * material.ambient;
    float Kd = max(dot(L,N),0.0);
    diffuse += Kd * spot.diffuse * material.diffuse;
    float Ks = pow(max(dot(R,E),0.0), material.shininess);
    specular += Ks * spot.specular * material.specular;
    if(dot(L,N) < 0.0) specular = vec4(0.0,0.0,0.0,1.0);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * v;
  }
  
  else {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * v;
  }
  
  mat_text = v_texture.z;
  texture = v_texture.xy;
}
