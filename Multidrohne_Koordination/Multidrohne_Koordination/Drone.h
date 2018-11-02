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
	float z; // Vorerst betrachten wir einen Bereich von 0 bis 100. Je nach die Größe dieses Bereich, normalisieren wir die Größe der Drohnen
			 // ihrer Höhe entsprechend
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

	void setColorGreen();					// setter funktion. setzt eine farbe für die drohne (weiß)
	void setColorWhite();					// setter Funktion. Setzt eine Farbe für die Drohne (Grün)

	float getZ();							// getter Funktion für die Flughöhe
	void setZ(float zSet);					// setter Funktion für die Flughöhe

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