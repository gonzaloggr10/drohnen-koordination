// INHALT: SOLUTION OHNE MAPE-K SCHICHT, DIE EINE 2D WELT DARSTELLT, IN DER MEHRERE KREISE (DROHNEN) SICH ZUFÄLLIG BEWEGEN.
// EINE DETEKTION DER GRUPPENFORMIERUNG WIRD DRUCHGEFÜHRT UND DARGESTELLT, INDEM DIE DROHNEN IN GRÜN GEFÄRBT WERDEN
#include "stdafx.h"
#include "Drone.h"
#include "Base.h"
#include "mape.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include <thread>

using namespace std;
using namespace sf;


int main()
{
	int thedata = 13;
	int windowWidth = 1024;
	int windowHeight = 768;
	// Fenster von 1024 mal 768 pixels erstellen
	// die den Titel ""Multidrohne Koordination"" hat
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Multidrohne Koordination");

	// Erstellung der Zentren
	vector<Base> listOfBases;
	vector<Base>::iterator itBases;

	Base *baseO = new Base(500, 550, 50);
	Base *baseA = new Base(150, 250, 50);
	Base *baseB = new Base(500, 150, 50);
	Base *baseC = new Base(850, 250, 50);

	listOfBases.push_back(*baseO);
	listOfBases.push_back(*baseA);
	listOfBases.push_back(*baseB);
	listOfBases.push_back(*baseC);

	// Definition von Variablen für Schnellzugriff zur Position der Zentren
	Vector2f *targetBaseO = new Vector2f(baseO->getPosition().x, baseO->getPosition().y);
	Vector2f *targetBaseA = new Vector2f(baseA->getPosition().x, baseA->getPosition().y);
	Vector2f *targetBaseB = new Vector2f(baseB->getPosition().x, baseB->getPosition().y);
	Vector2f *targetBaseC = new Vector2f(baseC->getPosition().x, baseC->getPosition().y);

	// Zielliste für jede Drohne definieren
	vector<Vector2f> listOfTargetsDrone0;
	vector<Vector2f> listOfTargetsDrone1;
	vector<Vector2f> listOfTargetsDrone2;
	vector<Vector2f> listOfTargetsDrone3;

	listOfTargetsDrone0.push_back(*targetBaseO);
	listOfTargetsDrone0.push_back(*targetBaseA);
	listOfTargetsDrone0.push_back(*targetBaseB);
	listOfTargetsDrone0.push_back(*targetBaseC);



	// Drohnenvektor erstellen
	vector<Drone> listOfDrones;
	vector<Drone>::iterator it;

	Drone *drone0 = new Drone(1000, 1, 55, listOfTargetsDrone0, 0, 1);
	Drone *drone1 = new Drone(1, 600, 55, listOfTargetsDrone0, 0, 3);
	Drone *drone2 = new Drone(500, 500, 55, listOfTargetsDrone0, 0, 5);
	Drone *drone3 = new Drone(1, 1, 55, listOfTargetsDrone0, 0, 6);
	Drone *drone4 = new Drone(500, 1, 55, listOfTargetsDrone0, 0, 4);

	listOfDrones.push_back(*drone0);
	listOfDrones.push_back(*drone1);
	listOfDrones.push_back(*drone2);
	listOfDrones.push_back(*drone3);
	listOfDrones.push_back(*drone4);

	// Zahl Drohnen bestimmen
	const vector<Drone>::size_type nd = listOfDrones.size();

	// Zahl Abstandsbererechnungen  bestimmen. Formell nb = nd * (nd - 1) / 2
	const int nb = nd * (nd - 1) / 2;

	// Zähler deklarieren
	int i, j;

	// Grenzwert für den Abstand
	int gw = 300;

	// Erstellung des Threads
	std::thread first(mape, &listOfDrones);
	
	//Diese while-Schleife ist "unendlich"
	while (window.isOpen())
	{
		/*
		Geht mit der Eingabe des Benutzers um
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				// Jemand hat das Fenster geschlossen
				window.close();
		}

		/*
		Frame updaten
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		// for-Schleife, die dabei hilft, das Update für all die Drohnen zu ermöglichen
		for (it = listOfDrones.begin(); it != listOfDrones.end(); ++it)
		{
			// Umgang mit dem Treffen der Unterseite oder Oberseite
			if (it->getPosition().top < 0 || it->getPosition().top + 10 > windowHeight)		// WICHTIG: Die addierte Zahl (10) entspricht der Höhe
			{
				// Richtung der Drohne umkehren
				it->reboundBottomOrTop();
			}
			// Umgang mit dem Treffen der Seiten
			if (it->getPosition().left < 0 || it->getPosition().left + 10 > windowWidth)	// WICHTIG: Die addierte Zahl (10) entspricht der Breite
			{
				it->reboundSides();
			}
			
			it->update();
		}		

		// Clear everything from the last frame

		window.clear(Color(Color::Black));

		// Zentren zeichnen
		for (itBases = listOfBases.begin(); itBases != listOfBases.end(); ++itBases)
		{
			window.draw(itBases->getShape());
		}

		// Drohnen und Texte zeichnen
		for (it = listOfDrones.begin(); it != listOfDrones.end(); ++it)
		{
			window.draw(it->getShape());
			window.draw(it->getDroneAura());
			window.draw(it->getText());
		}
		Sleep(5);
		// Alles zeigen, was wir gerade gezeichnet haben
		window.display();		
	} // Ende der while-Schleife
	std::cout << "\n\n\nFINISH\n\n\n" << std::endl;
	std::cout << "value of data" << thedata << std::endl;
	first.join();

	return 0;
}