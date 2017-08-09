//
//  Line.hpp
//  
//
//  Created by Bret Jackson on 2/3/17.
//
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>

#include "Mesh.h"
#include "GLSLProgram.h"
#define GLM_FORCE_RADIANS
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_access.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/quaternion.hpp>

namespace basicgraphics {

	class Line {
	public:

		/*!
		 * Create a line (i.e. a flat ribbon) from start to end. The ribbon surface will face the normal direction and have width of 2*radius
		 */
		Line(const glm::vec3 &start, const glm::vec3 &end, const glm::vec3 &normal, const float radius, const glm::vec4 &color);
		virtual ~Line();

		virtual void draw(GLSLProgram &shader, const glm::mat4 &modelMatrix);

		/*!
		* returns the closest point to on the line to pt
		*/
		glm::vec3 closestPoint(const glm::vec3 &pt) const;

	protected:
		std::unique_ptr<Mesh> _mesh;
		const glm::vec3 _start;
		const glm::vec3 _end;
        glm::vec3 _normal;
		const float _radius;
		const glm::vec4 _color;
	};

}

#endif /* Line_hpp */
