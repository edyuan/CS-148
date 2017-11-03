#include <common/Scene/Light/DirectionalLight.h>
#include "common/Scene/Light/LightProperties.h"
#include "common/Rendering/Shaders/ShaderProgram.h"


DirectionalLight::DirectionalLight(std::unique_ptr<struct LightProperties> inProperties) : Light(std::move(inProperties),LightType::DIRECTIONAL)
{
	LIGHT_UNIFORM_NAME = "directionalLight";
}

DirectionalLight::~DirectionalLight()
{
}


void DirectionalLight::SetupShaderUniforms(const ShaderProgram* program) const
{
	program->SetShaderUniform(LIGHT_UNIFORM_NAME + ".direction",this->GetForwardDirection());
}