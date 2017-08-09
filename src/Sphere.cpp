//
//  Sphere.cpp
//  
//
//  Created by Bret Jackson on 2/3/17.
//
//

#include "Sphere.h"

namespace basicgraphics {


	Sphere::Sphere(const glm::vec3 &position, const float radius, const glm::vec4 &color) : _position(position), _radius(radius), _color(color)
	{
        _model = getModelInstance();
	}

	Sphere::~Sphere()
	{

	}
    
    std::shared_ptr<Model> Sphere::getModelInstance(){
        static std::shared_ptr<Model> model(new Model("sphere.obj", 1.0, glm::vec4(1.0)));
        return model;
    }

	void Sphere::draw(GLSLProgram &shader, const glm::mat4 &modelMatrix) {

		glm::mat4 translate = glm::translate(glm::mat4(1.0), _position);
        glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(_radius));
		glm::mat4 model = modelMatrix * translate * scale;
		shader.setUniform("model_mat", model);
        shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
        _model->setMaterialColor(_color);
		_model->draw(shader);
		shader.setUniform("model_mat", modelMatrix);
        shader.setUniform("normal_mat", mat3(transpose(inverse(modelMatrix))));
	}

}