#pragma once

#include <cstdint>

#include <glm/glm.hpp>

class Color
{
	private:
		uint32_t _truncatedColor = 0xFF000000; // Black
		uint8_t _R() { return _truncatedColor && 0xFF; }
		uint8_t _G() { return _truncatedColor >>  8 && 0xFF; }
		uint8_t _B() { return _truncatedColor >> 16 && 0xFF; }
		uint8_t _A() { return _truncatedColor >> 24 && 0xFF; }

	public:
		constexpr Color(uint32_t value) : _truncatedColor(value) {}

		static const Color Red;
		
		glm::vec3 ToVector3() { return glm::vec3((float)this->_R() / 255,
												 (float)this->_G() / 255,
												 (float)this->_B() / 255);}

		glm::vec4 ToVector4() { return glm::vec4((float)this->_R() / 255,
			   									 (float)this->_G() / 255, 
			   									 (float)this->_B() / 255,
			   									 (float)this->_A() / 255); }
};

const Color Color::Red = Color(0xFF0000FF);