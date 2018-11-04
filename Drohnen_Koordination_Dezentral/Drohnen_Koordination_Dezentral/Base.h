#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Base {
private:
	Vector2f position;
	float radius;

	CircleShape baseShape;
public:
	Base();
	Base(float, float, float);

	Vector2f getPosition();
	CircleShape getShape();
	float getRadius();

	~Base();

};
