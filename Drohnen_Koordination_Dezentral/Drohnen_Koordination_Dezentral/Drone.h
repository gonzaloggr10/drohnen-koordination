#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

class Drone
{
private:
	Vector2f position;
	Vector2f positionText;
	Vector2f positionAura;
	Vector2f currTarget;
	vector<Vector2f> listOfTargets;
	vector<Vector2f>::iterator itT;
	float zGoal;
	float z; // Vorerst betrachten wir einen Bereich von 0 bis 100. Je nach die Gr��e dieses Bereich, normalisieren wir die Gr��e der Drohnen
			 // ihrer H�he entsprechend
	bool targetZActive;
	bool executeActive;
	bool flag;
	// Variable f�r's Gebiet-Tag der Drohne
	int areaTag;

	// Ein Objekt "droneShape" gennant der Klasse "RectangleShape" 
	RectangleShape droneShape;
	Text droneText;
	// Definition eines CircleShape-Objekts f�r die Darstellung der Auren
	CircleShape droneAura;
	int cDDrone;
	// Damit wir ein Text-Objekt erstellen k�nnen, brauchen wir ein Font
	Font font;
	// String-Variable f�r die Eingabe der H�he ins Text-Objekt
	string heightString;

	float xVelocity;
	float yVelocity;
	float zVelocity;
	int weight;
	int id;
	bool goalViolated;
	int currCorr;

public:
	//int weight; 
	//int id;

	Drone(float startX, float startY, float startZ, vector<Vector2f> tl, int ID, int intWeight, int critD);		// Konstruktor

	FloatRect getPosition();				// getter Funktion. Erhaltet die Position des Rechteckes vom droneShape Objekt

	RectangleShape getShape();				// getter Funktion. Erhaltet die Variable droneShape vom Typ RectangleShape

	CircleShape getDroneAura();

	Text getText();

	void setColorGreen();					// setter funktion. setzt eine farbe f�r die drohne (wei�)
	void setColorWhite();					// setter Funktion. Setzt eine Farbe f�r die Drohne (Gr�n)

	float getZ();							// getter Funktion f�r die Flugh�he
	void setZGoal(float zSetGoal);			// setter Funktion f�r die Flugh�he

	bool getTargetZActive();				// getter Funktion f�r targetZActive
	void setTargetZActive(bool);			// setter Funktion f�r targetZActive

	void setZVelocity();					// Funktion, die die Bewegung die Z-Achse entlang ausl�st

	bool getExecuteActive();
	void setExecuteActive(bool);

	float getXVelocity();

	int getWeight();

	bool getGoalViolated();
	void setGoalViolated(bool);

	int getCurrCorr();
	void setCurrCorr(int);

	int getAreaTag();
	void setAreaTag(int&);

	void setID(int);
	int getID();

	Vector2f calcVelocity(Vector2f&);

	void reboundSides();

	void reboundBottomOrTop();

	void update();

};