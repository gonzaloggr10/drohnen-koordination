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
	Vector2f positionWeight;
	Vector2f positionAura;
	Vector2f currTarget;
	vector<Vector2f> listOfTargets;
	vector<Vector2f>::iterator itT;
	vector<int> group;
	vector<int>::iterator itG;
	float zGoal;
	float z; // Vorerst betrachten wir einen Bereich von 0 bis 100. Je nach die Größe dieses Bereich, normalisieren wir die Größe der Drohnen
			 // ihrer Höhe entsprechend
	float gw;
	bool targetZActive;
	bool executeActive;
	bool flag;
	int groupTag;

	// Ein Objekt "droneShape" gennant der Klasse "RectangleShape" 
	RectangleShape droneShape;
	Text droneText;
	// Definition eines CircleShape-Objekts für die Darstellung der Auren
	CircleShape droneAura;
	// Damit wir ein Text-Objekt erstellen können, brauchen wir ein Font
	Font font;
	// String-Variable für die Eingabe der Höhe ins Text-Objekt
	string heightString;

	// ZEICHNEN DES GEWICHTS
	Text droneWeight;
	// String-Variable für die Eingabe der Höhe ins Text-Objekt
	string weightString;

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

	Drone(float startX, float startY, float startZ, vector<Vector2f> tl, int ID, int intWeight);		// Konstruktor
	
	FloatRect getPosition();				// getter Funktion. Erhaltet die Position des Rechteckes vom droneShape Objekt

	RectangleShape getShape();				// getter Funktion. Erhaltet die Variable droneShape vom Typ RectangleShape

	CircleShape getDroneAura();				

	Text getText();			

	Text getDroneWeight();

	// Diese Funktion ist einer setter-Funktion entsprechend
	void addDroneToGroup(int);
	vector<int> getGroup();					// setter Funktion für die Gruppe
	void printGroup();
	void eraseGroup();

	void setGroupTag(int &gTag);
	int getGroupTag();
	// Funktion, die üperprüft, ob eine Drohne in einer Gruppe bereits ist
	bool findDroneInGroup(int);


	void setColorBlue();					// setter funktion. setzt eine farbe für die drohne (weiß)
	void setColorWhite();					// setter Funktion. Setzt eine Farbe für die Drohne (Grün)

	float getZ();							// getter Funktion für die Flughöhe
	void setZGoal(float zSetGoal);			// setter Funktion für die Flughöhe

	bool getTargetZActive();				// getter Funktion für targetZActive
	void setTargetZActive(bool);			// setter Funktion für targetZActive

	void setZVelocity();					// Funktion, die die Bewegung die Z-Achse entlang auslöst

	bool getExecuteActive();
	void setExecuteActive(bool);

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