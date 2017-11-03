#include <common/Scene/Light/HemisphereLight.h>
#include "common/Scene/Light/LightProperties.h"
#include "common/Rendering/Shaders/ShaderProgram.h"
#include "common/core.h" // <-- haha.


HemisphereLight::HemisphereLight(std::unique_ptr<struct LightProperties> inProperties) : Light(std::move(inProperties),LightType::HEMISPHERE)
{
	LIGHT_UNIFORM_NAME = "hemisphereLight";
}

HemisphereLight::~HemisphereLight()
{
}


void HemisphereLight::SetupShaderUniforms(const ShaderProgram* program) const
{
	program->SetShaderUniform(LIGHT_UNIFORM_NAME + ".colorground",glm::vec4(1.f,0.f,0.f,1.f));
	program->SetShaderUniform(LIGHT_UNIFORM_NAME + ".colorsky", glm::vec4(0.f, 1.f, 0.f, 1.f));
}