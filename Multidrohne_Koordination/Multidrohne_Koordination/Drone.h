#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace sf;

class Drone
{
private:
	Vector2f position;
	Vector2f currTarget;
	vector<Vector2f> listOfTargets;
	vector<Vector2f>::iterator itT;
	float z; // Vorerst betrachten wir einen Bereich von 0 bis 100. Je nach die Gr��e dieses Bereich, normalisieren wir die Gr��e der Drohnen
			 // ihrer H�he entsprechend
	bool flag;

	// Ein Objekt "droneShape" gennant der Klasse "RectangleShape" 
	RectangleShape droneShape;

	float xVelocity;
	float yVelocity;
	int weight;
	int id;
	bool goalViolated;
	int currCorr;

public:
	//int weight; 
	//int id;

	Drone(float startX, float startY, float startZ, vector<Vector2f> tl, int ID, int intWeight);		// Konstruktor
	
	FloatRect getPosition();				// getter Funktion. Erhaltet die Position des Rechteckes vom droneShape Objekt

	RectangleShape getShape();				// getter Funktion. Erhaltet die Variable droneShape vom Typ RectangleShape

	void setColorGreen();					// setter funktion. setzt eine farbe f�r die drohne (wei�)
	void setColorWhite();					// setter Funktion. Setzt eine Farbe f�r die Drohne (Gr�n)

	float getZ();							// getter Funktion f�r die Flugh�he
	void setZ(float zSet);					// setter Funktion f�r die Flugh�he

	float getXVelocity();

	int getWeight();

	bool getGoalViolated();
	void setGoalViolated(bool);

	int getCurrCorr();
	void setCurrCorr(int);

	void setID(int);
	int getID();

	Vector2f calcVelocity(Vector2f&);

	void reboundSides();

	void reboundBottomOrTop();

	void update();

};