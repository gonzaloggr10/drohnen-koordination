#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Base {
private:
	Vector2f position;
	float side;

	RectangleShape baseShape;
public:
	Base();
	Base(float, float, float);

	Vector2f getPosition();
	RectangleShape getShape();
	/*float getRadius();*/

	~Base();

};
