#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) smooth in vec4 vVaryingColor;

layout(location = 0) out vec4 vFragColor;

void main( void ) {
   vFragColor = vVaryingColor;
}
