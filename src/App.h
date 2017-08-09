#ifndef App_h
#define App_h

#include "BaseApp.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "TurntableManipulator.h"

namespace basicgraphics {
class App : public BaseApp {
public:
  
    App(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight);
    ~App(){};
  
    void onRenderGraphics() override;
    void onEvent(std::shared_ptr<Event> event) override;

  
protected:
    
    double lastTime;
    double totalTime;
    
    virtual void reloadShaders();
    
    std::shared_ptr<Texture> diffuseRamp;
    std::shared_ptr<Texture> specularRamp;
    
    GLSLProgram shader;
    
    std::unique_ptr<Model> modelMesh;
    std::shared_ptr<TurntableManipulator> turntable;
    
    glm::vec4 lightPosition;
    bool drawLightVector;
    float diffuseOnOff;  // 1.0 when on, 0.0 when off
    float specularOnOff; // 1.0 when on, 0.0 when off
    float ambientOnOff;  // 1.0 when on, 0.0 when off
  
};
}
#endif