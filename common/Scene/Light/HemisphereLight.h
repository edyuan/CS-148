#pragma once


#include <common/Scene/Light/Light.h>

/*! \brief Generic light class.
 *
 *  Note that the "Light" class only holds the properties related to the orientation and shape of the camera in world space.
 *  The actual "light" of the Light object is stored within a LightProperties structure. The Light class acts as a point light.
 */
class HemisphereLight: public Light
{
	public:

    HemisphereLight(std::unique_ptr<struct LightProperties> inProperties);
    virtual ~HemisphereLight();
	virtual void SetupShaderUniforms(const class ShaderProgram* program) const;

	private:


};

