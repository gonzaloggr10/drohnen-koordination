#include "stdafx.h"
#include "Base.h"


Base::Base()
{
}

Base::Base(float xBase, float yBase, float r)
{
	position.x = xBase;
	position.y = yBase;
	this->radius = r;
	
	//Mit den Eingabedaten definieren wir den "CircleShape" Objekt an der Stelle
	this->baseShape.setPosition(position);
	this->baseShape.setRadius(radius);
	this->baseShape.setFillColor(Color(Color::Magenta));

}

Vector2f Base::getPosition()
{
	return this->position;
}

float Base::getRadius()
{
	return this->radius;
}

CircleShape Base::getShape()
{
	return this->baseShape;
}


Base::~Base()
{
}
