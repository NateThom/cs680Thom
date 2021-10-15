#version 330

smooth in vec3 color;
smooth in vec2 texture;

out vec4 frag_color;

uniform sampler2D sampler;

void main(void)
{
   //for the below comment out one or the other.  DO NOT USE BOTH
   //This is if a texture is available to be loaded
   frag_color = texture2D(sampler, texture) * vec4(color, 1.0);
   //This is for testing models without textures
   //frag_color = vec4(color.rgb, 1.0);
}
