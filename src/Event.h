
#ifndef EVENT_H
#define EVENT_H

#define GLM_FORCE_RADIANS
#include <glm/glm/glm.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <string>

namespace basicgraphics {

	/** Event class.  To keep things simple, there are no subclasses
	of Event.  The type of data that the event carries is interpreted
	differently based on the value of the type of the event.  Button
	Events are typically sent by devices as two separate
	EVENTTYPE_STANDARD Events, the first named ButtonName_down and
	then when the button is released ButtonName_up.
	*/
	class Event
	{
	public:

		enum EventType {
			EVENTTYPE_STANDARD = 0,        /// standard type, carries no additional info
			EVENTTYPE_1D = 1,              /// event that stores 1D data in a double
			EVENTTYPE_2D = 2,              /// stores two doubles
			EVENTTYPE_3D = 3,              /// stores three doubles
			EVENTTYPE_4D = 4,			   /// stores four doubles
			EVENTTYPE_MAT4 = 5, /// stores a mat4
			EVENTTYPE_MSG = 6              /// stores a std::string
		};

		Event(const std::string &name, const GLFWwindow* window);
		Event(const std::string &name, const double data, const GLFWwindow* window);
		Event(const std::string &name, const glm::dvec2 &data, const GLFWwindow* window);
		Event(const std::string &name, const glm::dvec3 &data, const GLFWwindow* window);
		Event(const std::string &name, const glm::dvec4 &data, const GLFWwindow* window);
		Event(const std::string &name, const glm::dmat4 &data, const GLFWwindow* window);
		Event(const std::string &name, const std::string &data, const GLFWwindow* window);

		virtual ~Event();

		std::string getName() const;
		EventType getType() const;
		const GLFWwindow* getWindow() const;

		double get1DData();
		glm::dvec2 get2DData();
		glm::dvec3 get3DData();
		glm::dvec4 get4DData();
		glm::dmat4 getMat4Data();
		std::string getMsgData();

		void rename(const std::string &newname);

	protected:
		std::string _name;
		int	_id;
		const GLFWwindow* _window;
		EventType _type;
		double _data1D;
		glm::dvec2 _data2D;
		glm::dvec3 _data3D;
		glm::dvec4 _data4D;
		glm::dmat4 _dataMat4;
		std::string _dataMsg;
	};

}
#endif

