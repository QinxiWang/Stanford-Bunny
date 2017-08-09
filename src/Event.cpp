#include "Event.h"

namespace basicgraphics {
	
Event::Event(const std::string &name, const GLFWwindow* window)
{ 
	_name = name;
	_type = EVENTTYPE_STANDARD;
	_window = window;
}

Event::Event(const std::string &name, const double data, const GLFWwindow* window)
{ 
	_name = name;
	_data1D = data;
	_type = EVENTTYPE_1D;
	_window = window;
}

Event::Event(const std::string &name, const glm::dvec2 &data, const GLFWwindow* window)
{ 
	_name = name;
	_data2D = data;
	_type = EVENTTYPE_2D;
	_window = window;
}

Event::Event(const std::string &name, const glm::dvec3 &data, const GLFWwindow* window)
{ 
	_name = name;
	_data3D = data;
	_type = EVENTTYPE_3D;
	_window = window;
}

Event::Event(const std::string &name, const glm::dvec4 &data, const GLFWwindow* window)
{ 
	_name = name;
	_data4D = data;
	_type = EVENTTYPE_4D;
	_window = window;
}


Event::Event(const std::string &name, const glm::dmat4 &data, const GLFWwindow* window)
{ 
	_name = name;
	_dataMat4 = data;
	_type = EVENTTYPE_MAT4;
	_window = window;
}

Event::Event(const std::string &name, const std::string &data, const GLFWwindow* window)
{ 
	_name = name;
	_dataMsg = data;
	_type = EVENTTYPE_MSG;
	_window = window;
}

Event::~Event()
{
}

void Event::rename(const std::string &newname)
{
	_name = newname;
}

std::string Event::getName() const
{
	return _name;
}

Event::EventType Event::getType() const 
{
	return _type; 
}

const GLFWwindow* Event::getWindow() const
{ 
	return _window;
}

double Event::get1DData()
{
	return _data1D;
}

glm::dvec2 Event::get2DData()
{
	return _data2D;
}

glm::dvec3	Event::get3DData()
{
	return _data3D;
}

glm::dvec4	Event::get4DData()
{
	return _data4D;
}

glm::dmat4	Event::getMat4Data()
{
	return _dataMat4;
}

std::string	Event::getMsgData()
{
	return _dataMsg;
}


} // end namespace


