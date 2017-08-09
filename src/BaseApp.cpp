//
//  BaseApp.cpp
//  
//
//  Created by Bret Jackson on 1/27/17.
//
//

#include "BaseApp.h"

namespace basicgraphics {

	glm::vec2 BaseApp::cursorPos(0);

	BaseApp::BaseApp(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight) {
		glfwSetErrorCallback(BaseApp::error_callback);

		if (!glfwInit()) {
			exit(EXIT_FAILURE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		// Create the window
		_window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
		if (!_window) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		// Setup event callbacks
		glfwSetKeyCallback(_window, BaseApp::key_callback);
		glfwSetWindowSizeCallback(_window, BaseApp::window_size_callback);
		glfwSetWindowPosCallback(_window, BaseApp::window_pos_callback);
		glfwSetCursorEnterCallback(_window, BaseApp::cursor_enter_callback);
		glfwSetCursorPosCallback(_window, BaseApp::cursor_position_callback);
		glfwSetMouseButtonCallback(_window, BaseApp::mouse_button_callback);
		glfwSetScrollCallback(_window, BaseApp::scroll_callback);

		glfwSetWindowUserPointer(_window, this);

		glfwMakeContextCurrent(_window);

		glfwGetFramebufferSize(_window, &_windowWidth, &_windowHeight);
		glfwGetWindowPos(_window, &_windowXPos, &_windowYPos);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glfwSwapInterval(1);

		//Turn on depth testing. This is an optimization so that triangle fragments that are further in depth than something already rendered are not processed
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		// Specify the background color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// Check for opengl errors
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << err << std::endl;
		}
	}

	BaseApp::~BaseApp() {
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void BaseApp::run() {
		while (!glfwWindowShouldClose(_window))
		{
			glViewport(0, 0, _windowWidth, _windowHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			onRenderGraphics();

			glfwSwapBuffers(_window);
			glfwPollEvents();
		}
	}

	void BaseApp::onRenderGraphics()
	{
		/*
			This is an example of how you might set up the basic parameters for the shader.
		*/
		// Setup the view matrix to set where the camera is located
		//glm::mat4 view = glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0));
		// Eye position in world coordinates.
		//glm::vec3 eye_world = glm::vec3(glm::column(glm::inverse(view), 3));
		// Setup the projection matrix so that things are rendered in perspective
		//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.01f, 100.0f);
		// Setup the model matrix
		//glm::mat4 model = glm::mat4(1.0);

		// Update shader variables
		//_shader.setUniform("view_mat", view);
		//_shader.setUniform("projection_mat", projection);
		//_shader.setUniform("model_mat", model);
		//_shader.setUniform("eye_world", eye_world);

	}

	void BaseApp::onEvent(std::shared_ptr<Event> event)
	{
	}

	void BaseApp::error_callback(int error, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
	}

	void BaseApp::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		string name = "kbd_" + getKeyName(key);

		if (mods) {
			name = name + "_" + getModsName(mods);
		}

		string value = getKeyValue(key, mods);

		name = name + "_" + getActionName(action);

		BaseApp* app = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
		std::shared_ptr<Event> newEvent(new Event(name, value, window));
		app->onEvent(newEvent);
	}

	void BaseApp::updateWindowSize()
	{
		glfwGetFramebufferSize(_window, &_windowWidth, &_windowHeight);
	}

	void BaseApp::updateWindowPosition(int x, int y)
	{
		_windowXPos = x;
		_windowYPos = y;
	}

	void BaseApp::window_size_callback(GLFWwindow* window, int width, int height)
	{
		BaseApp* app = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
		app->updateWindowSize();
	}

	void BaseApp::window_pos_callback(GLFWwindow* window, int xpos, int ypos)
	{
		BaseApp* app = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
		app->updateWindowPosition(xpos, ypos);
	}

	void BaseApp::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		string name = getButtonName(button);

		if (mods) {
			name = name + "_" + getModsName(mods);
		}

		name = name + "_" + getActionName(action);

		BaseApp* app = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
		std::shared_ptr<Event> newEvent(new Event(name, cursorPos, window));
		app->onEvent(newEvent);
	}

	void BaseApp::cursor_position_callback(GLFWwindow* window, double x, double y)
	{
		// Note: x and y can be outside the bounds of the window!
		string name = "mouse_pointer";
		BaseApp* app = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
		BaseApp::cursorPos.x = x;
		BaseApp::cursorPos.y = y;
		std::shared_ptr<Event> newEvent(new Event(name, cursorPos, window));
		app->onEvent(newEvent);
	}

	void BaseApp::cursor_enter_callback(GLFWwindow* window, int entered)
	{
		string name = "mouse_pointer_" + entered ? "entered" : "left";
		BaseApp* app = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
		std::shared_ptr<Event> newEvent(new Event(name, window));
		app->onEvent(newEvent);
	}

	void BaseApp::scroll_callback(GLFWwindow* window, double x, double y)
	{
		string name = "mouse_scroll";
		BaseApp* app = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
		std::shared_ptr<Event> newEvent(new Event(name, glm::dvec2(x, y), window));
		app->onEvent(newEvent);
	}

	string BaseApp::getKeyName(int key)
	{
		switch (key)
		{
			// Printable keys
		case GLFW_KEY_A:            return "A";
		case GLFW_KEY_B:            return "B";
		case GLFW_KEY_C:            return "C";
		case GLFW_KEY_D:            return "D";
		case GLFW_KEY_E:            return "E";
		case GLFW_KEY_F:            return "F";
		case GLFW_KEY_G:            return "G";
		case GLFW_KEY_H:            return "H";
		case GLFW_KEY_I:            return "I";
		case GLFW_KEY_J:            return "J";
		case GLFW_KEY_K:            return "K";
		case GLFW_KEY_L:            return "L";
		case GLFW_KEY_M:            return "M";
		case GLFW_KEY_N:            return "N";
		case GLFW_KEY_O:            return "O";
		case GLFW_KEY_P:            return "P";
		case GLFW_KEY_Q:            return "Q";
		case GLFW_KEY_R:            return "R";
		case GLFW_KEY_S:            return "S";
		case GLFW_KEY_T:            return "T";
		case GLFW_KEY_U:            return "U";
		case GLFW_KEY_V:            return "V";
		case GLFW_KEY_W:            return "W";
		case GLFW_KEY_X:            return "X";
		case GLFW_KEY_Y:            return "Y";
		case GLFW_KEY_Z:            return "Z";
		case GLFW_KEY_1:            return "1";
		case GLFW_KEY_2:            return "2";
		case GLFW_KEY_3:            return "3";
		case GLFW_KEY_4:            return "4";
		case GLFW_KEY_5:            return "5";
		case GLFW_KEY_6:            return "6";
		case GLFW_KEY_7:            return "7";
		case GLFW_KEY_8:            return "8";
		case GLFW_KEY_9:            return "9";
		case GLFW_KEY_0:            return "0";
		case GLFW_KEY_SPACE:        return "SPACE";
		case GLFW_KEY_MINUS:        return "MINUS";
		case GLFW_KEY_EQUAL:        return "EQUAL";
		case GLFW_KEY_LEFT_BRACKET: return "LEFT_BRACKET";
		case GLFW_KEY_RIGHT_BRACKET: return "RIGHT_BRACKET";
		case GLFW_KEY_BACKSLASH:    return "BACKSLASH";
		case GLFW_KEY_SEMICOLON:    return "SEMICOLON";
		case GLFW_KEY_APOSTROPHE:   return "APOSTROPHE";
		case GLFW_KEY_GRAVE_ACCENT: return "GRAVE_ACCENT";
		case GLFW_KEY_COMMA:        return "COMMA";
		case GLFW_KEY_PERIOD:       return "PERIOD";
		case GLFW_KEY_SLASH:        return "SLASH";
		case GLFW_KEY_WORLD_1:      return "WORLD1";
		case GLFW_KEY_WORLD_2:      return "WORLD2";

			// Function keys
		case GLFW_KEY_ESCAPE:       return "ESC";
		case GLFW_KEY_F1:           return "F1";
		case GLFW_KEY_F2:           return "F2";
		case GLFW_KEY_F3:           return "F3";
		case GLFW_KEY_F4:           return "F4";
		case GLFW_KEY_F5:           return "F5";
		case GLFW_KEY_F6:           return "F6";
		case GLFW_KEY_F7:           return "F7";
		case GLFW_KEY_F8:           return "F8";
		case GLFW_KEY_F9:           return "F9";
		case GLFW_KEY_F10:          return "F10";
		case GLFW_KEY_F11:          return "F11";
		case GLFW_KEY_F12:          return "F12";
		case GLFW_KEY_F13:          return "F13";
		case GLFW_KEY_F14:          return "F14";
		case GLFW_KEY_F15:          return "F15";
		case GLFW_KEY_F16:          return "F16";
		case GLFW_KEY_F17:          return "F17";
		case GLFW_KEY_F18:          return "F18";
		case GLFW_KEY_F19:          return "F19";
		case GLFW_KEY_F20:          return "F20";
		case GLFW_KEY_F21:          return "F21";
		case GLFW_KEY_F22:          return "F22";
		case GLFW_KEY_F23:          return "F23";
		case GLFW_KEY_F24:          return "F24";
		case GLFW_KEY_F25:          return "F25";
		case GLFW_KEY_UP:           return "UP";
		case GLFW_KEY_DOWN:         return "DOWN";
		case GLFW_KEY_LEFT:         return "LEFT";
		case GLFW_KEY_RIGHT:        return "RIGHT";
		case GLFW_KEY_LEFT_SHIFT:   return "LEFT_SHIFT";
		case GLFW_KEY_RIGHT_SHIFT:  return "RIGHT_SHIFT";
		case GLFW_KEY_LEFT_CONTROL: return "LEFT_CONTROL";
		case GLFW_KEY_RIGHT_CONTROL: return "RIGHT_CONTROL";
		case GLFW_KEY_LEFT_ALT:     return "LEFT_ALT";
		case GLFW_KEY_RIGHT_ALT:    return "RIGHT_ALT";
		case GLFW_KEY_TAB:          return "TAB";
		case GLFW_KEY_ENTER:        return "ENTER";
		case GLFW_KEY_BACKSPACE:    return "BACKSPACE";
		case GLFW_KEY_INSERT:       return "INSERT";
		case GLFW_KEY_DELETE:       return "DELETE";
		case GLFW_KEY_PAGE_UP:      return "PAGEUP";
		case GLFW_KEY_PAGE_DOWN:    return "PAGEDOWN";
		case GLFW_KEY_HOME:         return "HOME";
		case GLFW_KEY_END:          return "END";
		case GLFW_KEY_KP_0:         return "KEYPAD0";
		case GLFW_KEY_KP_1:         return "KEYPAD1";
		case GLFW_KEY_KP_2:         return "KEYPAD2";
		case GLFW_KEY_KP_3:         return "KEYPAD3";
		case GLFW_KEY_KP_4:         return "KEYPAD4";
		case GLFW_KEY_KP_5:         return "KEYPAD5";
		case GLFW_KEY_KP_6:         return "KEYPAD6";
		case GLFW_KEY_KP_7:         return "KEYPAD7";
		case GLFW_KEY_KP_8:         return "KEYPAD8";
		case GLFW_KEY_KP_9:         return "KEYPAD9";
		case GLFW_KEY_KP_DIVIDE:    return "KEYPAD_DIVIDE";
		case GLFW_KEY_KP_MULTIPLY:  return "KEYPAD_MULTIPLY";
		case GLFW_KEY_KP_SUBTRACT:  return "KEYPAD_SUBTRACT";
		case GLFW_KEY_KP_ADD:       return "KEYPAD_ADD";
		case GLFW_KEY_KP_DECIMAL:   return "KEYPAD_DECIMAL";
		case GLFW_KEY_KP_EQUAL:     return "KEYPAD_EQUAL";
		case GLFW_KEY_KP_ENTER:     return "KEYPAD_ENTER";
		case GLFW_KEY_PRINT_SCREEN: return "PRINT_SCREEN";
		case GLFW_KEY_NUM_LOCK:     return "NUM_LOCK";
		case GLFW_KEY_CAPS_LOCK:    return "CAPS_LOCK";
		case GLFW_KEY_SCROLL_LOCK:  return "SCROLL_LOCK";
		case GLFW_KEY_PAUSE:        return "PAUSE";
		case GLFW_KEY_LEFT_SUPER:   return "LEFT_SUPER";
		case GLFW_KEY_RIGHT_SUPER:  return "RIGHT_SUPER";
		case GLFW_KEY_MENU:         return "MENU";
		case GLFW_KEY_UNKNOWN:      return "UNKNOWN";

		default:                    return "";
		}
	}

	string BaseApp::getKeyValue(int key, int mods)
	{
		string value = "";
		switch (key)
		{
			// Printable keys
		case GLFW_KEY_A:            value = "a"; break;
		case GLFW_KEY_B:            value = "b"; break;
		case GLFW_KEY_C:            value = "c"; break;
		case GLFW_KEY_D:            value = "d"; break;
		case GLFW_KEY_E:            value = "e"; break;
		case GLFW_KEY_F:            value = "f"; break;
		case GLFW_KEY_G:            value = "g"; break;
		case GLFW_KEY_H:            value = "h"; break;
		case GLFW_KEY_I:            value = "i"; break;
		case GLFW_KEY_J:            value = "j"; break;
		case GLFW_KEY_K:            value = "k"; break;
		case GLFW_KEY_L:            value = "l"; break;
		case GLFW_KEY_M:            value = "m"; break;
		case GLFW_KEY_N:            value = "n"; break;
		case GLFW_KEY_O:            value = "o"; break;
		case GLFW_KEY_P:            value = "p"; break;
		case GLFW_KEY_Q:            value = "q"; break;
		case GLFW_KEY_R:            value = "r"; break;
		case GLFW_KEY_S:            value = "s"; break;
		case GLFW_KEY_T:            value = "t"; break;
		case GLFW_KEY_U:            value = "u"; break;
		case GLFW_KEY_V:            value = "v"; break;
		case GLFW_KEY_W:            value = "w"; break;
		case GLFW_KEY_X:            value = "x"; break;
		case GLFW_KEY_Y:            value = "y"; break;
		case GLFW_KEY_Z:            value = "z"; break;

		default:					value = ""; break;
		}

		if (value != "") {
			if (mods & GLFW_MOD_SHIFT)
			{
				value = std::toupper(value[0]);
			}
		}
		else {

			switch (key)
			{
			case GLFW_KEY_1:            value = "1"; break;
			case GLFW_KEY_2:            value = "2"; break;
			case GLFW_KEY_3:            value = "3"; break;
			case GLFW_KEY_4:            value = "4"; break;
			case GLFW_KEY_5:            value = "5"; break;
			case GLFW_KEY_6:            value = "6"; break;
			case GLFW_KEY_7:            value = "7"; break;
			case GLFW_KEY_8:            value = "8"; break;
			case GLFW_KEY_9:            value = "9"; break;
			case GLFW_KEY_0:            value = "0"; break;
			case GLFW_KEY_SPACE:        value = " "; break;
			case GLFW_KEY_MINUS:        value = "-"; break;
			case GLFW_KEY_EQUAL:        value = "="; break;
			case GLFW_KEY_LEFT_BRACKET: value = "]"; break;
			case GLFW_KEY_RIGHT_BRACKET: value = "["; break;
			case GLFW_KEY_BACKSLASH:    value = "\\"; break;
			case GLFW_KEY_SEMICOLON:    value = ";"; break;
			case GLFW_KEY_APOSTROPHE:   value = "'"; break;
			case GLFW_KEY_GRAVE_ACCENT: value = "`"; break;
			case GLFW_KEY_COMMA:        value = ","; break;
			case GLFW_KEY_PERIOD:       value = "."; break;
			case GLFW_KEY_SLASH:        value = "/"; break;
			case GLFW_KEY_TAB:          value = "\t"; break;
			case GLFW_KEY_ENTER:        value = "\n"; break;
			case GLFW_KEY_KP_0:         value = "0"; break;
			case GLFW_KEY_KP_1:         value = "1"; break;
			case GLFW_KEY_KP_2:         value = "2"; break;
			case GLFW_KEY_KP_3:         value = "3"; break;
			case GLFW_KEY_KP_4:         value = "4"; break;
			case GLFW_KEY_KP_5:         value = "5"; break;
			case GLFW_KEY_KP_6:         value = "6"; break;
			case GLFW_KEY_KP_7:         value = "7"; break;
			case GLFW_KEY_KP_8:         value = "8"; break;
			case GLFW_KEY_KP_9:         value = "9"; break;
			case GLFW_KEY_KP_DIVIDE:    value = "/"; break;
			case GLFW_KEY_KP_MULTIPLY:  value = "*"; break;
			case GLFW_KEY_KP_SUBTRACT:  value = "-"; break;
			case GLFW_KEY_KP_ADD:       value = "+"; break;
			case GLFW_KEY_KP_DECIMAL:   value = "."; break;
			case GLFW_KEY_KP_EQUAL:     value = "="; break;
			case GLFW_KEY_KP_ENTER:     value = "\n"; break;

			default:                    break;
			}
		}

		return value;
	}

	string BaseApp::getActionName(int action)
	{
		switch (action)
		{
		case GLFW_PRESS:
			return "down";
		case GLFW_RELEASE:
			return "up";
		case GLFW_REPEAT:
			return "repeat";
		}

		return "caused unknown action";
	}

	string BaseApp::getButtonName(int button)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			return "mouse_btn_left";
		case GLFW_MOUSE_BUTTON_RIGHT:
			return "mouse_btn_right";
		case GLFW_MOUSE_BUTTON_MIDDLE:
			return "mouse_btn_middle";
		default:
			std::stringstream ss;
			ss << "mouse_btn_" << button;
			return ss.str();
		}

		return "";
	}

	string BaseApp::getModsName(int mods)
	{
		string modName = "";
		if (mods & GLFW_MOD_SHIFT)
			modName += "SHIFT";
		if (mods & GLFW_MOD_CONTROL)
			modName += "CTRL";
		if (mods & GLFW_MOD_ALT)
			modName += "ALT";
		if (mods & GLFW_MOD_SUPER)
			modName += "SUPER";

		return modName;
	}

}


