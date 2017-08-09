#include "App.h"
#include <iostream>

namespace basicgraphics{

using namespace std;
using namespace glm;

App::App(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight) : BaseApp(argc, argv, windowName, windowWidth, windowHeight) {

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // This load shaders from disk, we do it once when the program starts
    // up, but since you don't need to recompile to reload shaders, you can
    // even do this inteactively as you debug your shaders!  Press the R
    // key to reload them while your program is running!
    reloadShaders();
    
    // This loads the model from a file and initializes an instance of the model class to store it
    modelMesh.reset(new Model("bunny.obj", 1.0, vec4(1.0)));
    
    //Loading textures
    diffuseRamp = Texture::create2DTextureFromFile("lightingNormal.jpg");
    diffuseRamp->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    diffuseRamp->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    specularRamp = Texture::create2DTextureFromFile("lightingNormal.jpg");
    specularRamp->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    specularRamp->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    turntable.reset(new TurntableManipulator(3, 0.3, 0.5));
    turntable->setCenterPosition(vec3(-0.3,0.8,0));
    
    drawLightVector = false;
    ambientOnOff = 1.0;
    diffuseOnOff = 1.0;
    specularOnOff = 1.0;
    totalTime = 0.0;
    
}
    
void App::reloadShaders()
{
    shader.compileShader("BlinnPhong.vert", GLSLShader::VERTEX);
    shader.compileShader("BlinnPhong.frag", GLSLShader::FRAGMENT);
    shader.link();
    shader.use();
}
    

void App::onEvent(shared_ptr<Event> event)
{
    string name = event->getName();
    
    // Dolly the camera closer or farther away from the earth
    if (name == "kbd_R_down") {
        reloadShaders();
    }
    else if (name == "kbd_L_down") {
        drawLightVector = !drawLightVector; // Toggle drawing the vector to the light on or off
    }
    // Press S to toggle specular lighting on/off
    else if (name == "kbd_S_down") {
        if (specularOnOff == 1.0) {
            specularOnOff = 0.0;
        }
        else {
            specularOnOff = 1.0;
        }
    }
    // Press D to toggle diffuse lighting on/off
    else if (name == "kbd_D_down") {
        if (diffuseOnOff == 1.0) {
            diffuseOnOff = 0.0;
        }
        else {
            diffuseOnOff = 1.0;
        }
    }
    // Press A to toggle ambient lighting on/off
    if (name == "kbd_A_down") {
        if (ambientOnOff == 1.0) {
            ambientOnOff = 0.0;
        }
        else {
            ambientOnOff = 1.0;
        }
    }

    
    turntable->onEvent(event);
}

void App::onRenderGraphics() {
    double currentT = glfwGetTime();
    totalTime += 0.25*(currentT - lastTime);
    lastTime = currentT;
    
    // Make the light orbit around the bunny so we can see the lighting change in response to the light position
    float radius = 5.0;
    lightPosition = vec4(cos(totalTime*0.6)*sin(totalTime*0.5)*radius,
                            cos(totalTime*0.3)*sin(totalTime*0.2)*radius,
                            cos(totalTime*0.1)*sin(totalTime*0.4)*radius,
                            1.0);
    
    
    // Setup the camera with a good initial position and view direction to see the table
    glm::mat4 view = turntable->frame();
    glm:mat4 model(1.0);
    // Setup the projection matrix so that things are rendered in perspective
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.1f, 100.0f);
    shader.use(); // Tell opengl we want to use this specific shader.
    shader.setUniform("view_mat", view);
    shader.setUniform("projection_mat", projection);
    shader.setUniform("model_mat", model);
    shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
    vec3 eyePosition = turntable->getPos();
    shader.setUniform("eye_world", eyePosition);
    diffuseRamp->bind(0);
    shader.setUniform("diffuseRamp", 0);
    specularRamp->bind(1);
    shader.setUniform("specularRamp", 1);
    
    
    // Properties of the material the model is made out of (the "K" terms in the equations discussed in class)
    // These values should make the model look like it is made out of a metal, like brass
    
    vec3 ambientReflectionCoeff = vec3(0.4125, 0.275, 0.0375);
    vec3 diffuseReflectionCoeff = vec3(0.78, 0.57, 0.11);
    vec3 specularReflectionCoeff = vec3(0.99, 0.94, 0.80);
    float specularExponent = 27.9;
    
    
    // For toon shading, you want all the color to come from the texture, so you can just use a white bunny like this:
    
//     vec3 ambientReflectionCoeff = vec3(1,1,1);
//     vec3 diffuseReflectionCoeff = vec3(1,1,1);
//     vec3 specularReflectionCoeff = vec3(1,1,1);
//     float specularExponent = 50.0;
    
    
    // Properties of the light source (the "I" terms in the equations discussed in class)
    // These values are for a white light so the r,g,b intensities are all the same
    // Note: lightPosition is another important property of the light; it is set at the top of the method
    vec3 ambientLightIntensity = vec3(0.4, 0.4, 0.4);
    vec3 diffuseLightIntensity = vec3(0.6, 0.6, 0.6);
    vec3 specularLightIntensity = vec3(1.0, 1.0, 1.0);
    
    // Multiply these light intensities by the OnOff terms below to turn each commonent on the lighting on/off based on keystrokes
    ambientLightIntensity *= ambientOnOff;
    diffuseLightIntensity *= diffuseOnOff;
    specularLightIntensity *= specularOnOff;
    
    
    // TODO: Pass these parameters into your shader programs... in shader programs these are called "uniform variables"
    
    shader.setUniform("lightPosition", lightPosition);
    shader.setUniform("specularExponent", specularExponent);
    
    shader.setUniform("ambientReflectionCoeff", ambientReflectionCoeff);
    shader.setUniform("diffuseReflectionCoeff", diffuseReflectionCoeff);
    shader.setUniform("specularReflectionCoeff",specularReflectionCoeff);
    
    shader.setUniform("ambientLightIntensity", ambientLightIntensity);
    shader.setUniform("diffuseLightIntensity", diffuseLightIntensity);
    shader.setUniform("specularLightIntensity",specularLightIntensity);
    

    // Draw the model
    modelMesh->draw(shader);
    
    // For debugging purposes, let's draw a sphere to reprsent each "light bulb" in the scene, that way
    // we can make sure the lighting on the bunny makes sense given the position of each light source.
    Sphere s(vec3(lightPosition), 0.1, vec4(1.0, 1.0, 0.0, 1.0));
    s.draw(shader, model);
    
    // Another useful aid for debugging: draw vectors to the light sources
    if (drawLightVector) {
        vec3 toLight = vec3(lightPosition) - vec3(-0.3,0.8,0);
        vec3 normal = normalize(cross(toLight, cross(toLight, vec3(0,1,0))));
        Line l(vec3(-0.3,0.8,0), vec3(lightPosition), normal, 0.01, vec4(1.0, 1.0, 0.0, 1.0));
        l.draw(shader, model);
    }


}
}//namespace




