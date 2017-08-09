#include "TurntableManipulator.h"

using namespace std;
using namespace glm;

namespace basicgraphics{

const double X_SCALE = 0.0001;
const double Y_SCALE = 0.0001;

TurntableManipulator::TurntableManipulator(double d, double a, double u) {
	distance = d;
	around = a;
	up = u;
    
    mouseDown = false;
}

void TurntableManipulator::bump(double ar, double u) {
	around += ar;
	up += u;
}

void TurntableManipulator::onEvent(shared_ptr<Event> event) {
    
    string name = event->getName();
    
    if (name == "kbd_UP_down" || name == "kbd_UP_repeat") {
        distance -= 0.01;
    }
    else if (name == "kbd_DOWN_down" || name == "kbd_DOWN_repeat") {
        distance += 0.01;
    }
    else if (name == "mouse_btn_left_down") {
        mouseDown = true;
        lastMousePos = event->get2DData();
    }
    else if (name == "mouse_btn_left_up") {
        mouseDown = false;
    }
    else if (name == "mouse_pointer"){
        if (mouseDown){
            vec2 dxy = vec2(event->get2DData()) - lastMousePos;
            if(length(dxy) != 0.0){
                around = around + dxy.x * X_SCALE;
                up = clamp(up + dxy.y * Y_SCALE, -pi<float>()/2 + 0.2, pi<float>()/2 - 0.2);
            }
        }
    }
}

glm::vec3 TurntableManipulator::getPos() const {
    glm::vec3 direction(cos(around)*cos(up)*distance,
                  sin(up)*distance,
                  sin(around)*cos(up)*distance);
    return center + direction;
}

glm::mat4 TurntableManipulator::frame() const {
    glm::vec3 pos = getPos();
    glm::mat4 cf = glm::lookAt(pos, center, glm::vec3(0, 1, 0));
	return cf;
}


void TurntableManipulator::setCenterPosition(glm::vec3 position) {
	center = position;
}
}//namespace
