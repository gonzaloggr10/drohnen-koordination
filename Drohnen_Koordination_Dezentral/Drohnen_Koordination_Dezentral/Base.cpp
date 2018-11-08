#include "stdafx.h"
#include "Base.h"


Base::Base()
{
}

Base::Base(float xBase, float yBase, float s)
{
	position.x = xBase;
	position.y = yBase;
	this->side = s;

	//Mit den Eingabedaten definieren wir den "CircleShape" Objekt an der Stelle
	this->baseShape.setPosition(position);
	//this->baseShape.setRadius(radius);
	this->baseShape.setSize(Vector2f(s, s)); // 90 ist die maximale Größe und 100 ist die Normalisierungskoeffizient
	this->baseShape.setFillColor(Color(Color::Magenta));

}

Vector2f Base::getPosition()
{
	return this->position;
}

//float Base::getRadius()
//{
//	return this->radius;
//}
//
RectangleShape Base::getShape()
{
	return this->baseShape;
}


Base::~Base()
{
}
