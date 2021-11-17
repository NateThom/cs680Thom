#version 330

in vec2 texture;
in vec4 ambient;
in vec4 diffuse;
in vec4 specular;
in float mat_text;

out vec4 fragColor;

uniform sampler2D sampler;

void main(void)
{
   if(mat_text == 0.0){
     vec4 textureColor = texture2D(sampler, texture);
     fragColor = (ambient + diffuse) * textureColor + specular;
   }
   else{
     fragColor = ambient + diffuse + specular;
   }
}
