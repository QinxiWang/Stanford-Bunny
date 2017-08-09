/*!
 *  BaseApp.h
 *  
 * This represents the main application. It creates an opengl window and starts rendering when run() is called.
 */

#ifndef BaseApp_h
#define BaseApp_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

#define GLM_FORCE_RADIANS
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_access.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "GLSLProgram.h"
#include "Model.h"
#include "Event.h"
#include "Sphere.h"
#include "Line.h"

namespace basicgraphics {

	/** BaseApp is a simple way to create a graphics application.  Developers
	need only to subclass BaseApp and override the onEvent(...), onRenderGraphics(...) member functions.
	To run the application, developers can call run.
	For example here is how
	to create a simple Graphics application:
	------------------------------------------------------------------------
	#include "BaseApp.h"
	class MyApp : public BaseApp {
	public:
		MyApp(int argc, char** argv,  std::string windowName, int windowWidth, int windowHeight) : BaseApp(argc, argv, windowName, windowWidth, windowHeight) {}
	   virtual void onRenderGraphics() {
		   // draw graphics
	   }
	};
	int main(int argc, char **argv) {
	   MyVRApp app(argc, argv);
		app.run();
	   return 0;
	}
	------------------------------------------------------------------------
	*/
	class BaseApp
	{
	public:

		BaseApp(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight);
		virtual ~BaseApp();

		/*!
		 * Starts the main draw loop
		 */
		void run();

		// Callbacks for user input
		static void error_callback(int error, const char* description);
		static void window_size_callback(GLFWwindow* window, int width, int height);
		static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double x, double y);
		static void cursor_enter_callback(GLFWwindow* window, int entered);
		static void scroll_callback(GLFWwindow* window, double x, double y);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static glm::vec2 cursorPos;

	protected:
		GLFWwindow* _window; // Pointer to the window
		int _windowWidth;  //This is based on the framebuffer size, not the windowWidth passed to the constructor
		int _windowHeight; //This is based on the framebuffer size, not the windowWidth passed to the constructor
		int _windowXPos;
		int _windowYPos;


		/*!
		 * Called from the run loop. You should put any of your drawing code in this member function.
		 */
		virtual void onRenderGraphics();

		/*!
		* Events are defined by a name stored as a std::string and some associated data.
		* The name explains the source and event. For example, an event
		* named "mouse_btn_left_down" tells you all you need to know.
		* In your code, you might do something like the following:
		*
		* void onEvent(const std::shared_ptr<Event> event) {
		*	if (event->getName() == "mouse_btn_left_down") {
		*		// do something with the mouse click
		*	}
		* }
		*/
		virtual void onEvent(const std::shared_ptr<Event> event);

		// Called from the event callbacks to update the member variables
		void updateWindowSize();
		void updateWindowPosition(int x, int y);

		// Keypress helper methods
		static std::string getKeyName(int key);
		static std::string getKeyValue(int key, int mods);
		static std::string getActionName(int action);
		static std::string getButtonName(int button);
		static std::string getModsName(int mods);

	};

}

#endif /* BaseApp_h */
