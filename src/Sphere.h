//
//  Sphere.hpp
//  
//
//  Created by Bret Jackson on 2/3/17.
//
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include "Model.h"
#include "GLSLProgram.h"
#define GLM_FORCE_RADIANS
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_access.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

namespace basicgraphics {

	class Sphere {
	public:

		/*!
		 * Creates a sphere centered at position with the specified radius and color
		 */
		Sphere(const glm::vec3 &position, const float radius, const glm::vec4 &color);
		virtual ~Sphere();

		virtual void draw(GLSLProgram &shader, const glm::mat4 &modelMatrix);

	protected:
        std::shared_ptr<Model> _model;
		const glm::vec3 _position;
		const float _radius;
		const glm::vec4 _color;
        
        std::shared_ptr<Model> getModelInstance();
	};
    
}

#endif /* Sphere_hpp */
