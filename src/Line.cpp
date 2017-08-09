//
//  Line.cpp
//  
//
//  Created by Bret Jackson on 2/3/17.
//
//

#include "Line.h"

namespace basicgraphics {

	Line::Line(const glm::vec3 &start, const glm::vec3 &end, const glm::vec3 &normal, const float radius, const glm::vec4 &color) : _start(start), _end(end), _radius(radius), _color(color)
	{
		_normal = glm::normalize(normal);
		glm::vec3 direction = _end - _start;
		glm::vec3 right = glm::normalize(glm::cross(normal, direction));

		std::vector<Mesh::Vertex> cpuVertexArray;
		std::vector<int>			 cpuIndexArray;
		std::vector<std::shared_ptr<Texture>> textures;

		Mesh::Vertex vert;
		vert.position = _start + radius * right;
		vert.normal = _normal;
		vert.texCoord0 = glm::vec2(0, 0);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(0);

		vert.position = _start - radius * right;
		vert.normal = _normal;
		vert.texCoord0 = glm::vec2(0, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(1);

		vert.position = _end + radius * right;
		vert.normal = _normal;
		vert.texCoord0 = glm::vec2(1, 0);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(2);

		vert.position = _end - radius * right;
		vert.normal = _normal;
		vert.texCoord0 = glm::vec2(1, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(3);

		const int numVertices = cpuVertexArray.size();
		const int cpuVertexByteSize = sizeof(Mesh::Vertex) * numVertices;
		const int cpuIndexByteSize = sizeof(int) * cpuIndexArray.size();
		_mesh.reset(new Mesh(textures, GL_TRIANGLE_STRIP, GL_STATIC_DRAW, cpuVertexByteSize, cpuIndexByteSize, 0, cpuVertexArray, cpuIndexArray.size(), cpuIndexByteSize, &cpuIndexArray[0]));
		_mesh->setMaterialColor(_color);

	}

	Line::~Line()
	{
	}

	void Line::draw(GLSLProgram &shader, const glm::mat4 &modelMatrix)
	{
        shader.setUniform("model_mat", modelMatrix);
        shader.setUniform("normal_mat", mat3(transpose(inverse(modelMatrix))));
		_mesh->draw(shader);
	}

	glm::vec3 Line::closestPoint(const glm::vec3 &pt) const{
		// The vector from the end of the capsule to the point in question.
		vec3 v(pt - _start);

		// Projection of v onto the line segment scaled by 
		// the length of direction.
		vec3 direction(_end - _start);
		float t = dot(direction, v);

		// Avoid some square roots.  Derivation:
		//    t/direction.length() <= direction.length()
		//      t <= direction.squaredLength()

		if ((t >= 0) && (t <= glm::length2(direction))) {

			// The point falls within the segment.  Normalize direction,
			// divide t by the length of direction.
			return _start + direction * t / glm::length2(direction);

		}
		else {

			// The point does not fall within the segment; see which end is closer.

			// Distance from 0, squared
			float d0Squared = length2(v);

			// Distance from 1, squared
			float d1Squared = length2(v - direction);

			if (d0Squared < d1Squared) {

				// Point 0 is closer
				return _start;

			}
			else {

				// Point 1 is closer
				return _end;

			}
		}
	}
}