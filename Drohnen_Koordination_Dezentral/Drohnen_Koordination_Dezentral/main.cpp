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

// Deklaration globaler Variablen
float windowWidth = 1366;
float windowHeight = 768;

// Definition der Dimensionen der Gebietsmatrix
int rows = 2;
int cols = 3;
float windowHStep = windowWidth / cols;
float windowVStep = windowHeight / rows;

// Grenzwert für den Abstand
int criticalDistance = 200;


int main()
{
	// Fenster von 1366 mal 768 pixels erstellen
	// die den Titel ""Drohnen-Koordination"" hat
	RenderWindow window(VideoMode(int(windowWidth), int(windowHeight)), "Drohnen-Koordination");

	// Erstellung der Zentren
	vector<Base> listOfBases;
	vector<Base>::iterator itBases;

	Base *baseO = new Base(683, 550, 50);
	Base *baseA = new Base(350, 250, 50);
	Base *baseB = new Base(683, 150, 50);
	Base *baseC = new Base(1000, 250, 50);

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

	Drone *drone0 = new Drone(1000, 1, 55, listOfTargetsDrone0, 0, 1, criticalDistance);
	Drone *drone1 = new Drone(1, 600, 55, listOfTargetsDrone0, 0, 3, criticalDistance);
	Drone *drone2 = new Drone(500, 500, 55, listOfTargetsDrone0, 0, 5, criticalDistance);
	Drone *drone3 = new Drone(1, 1, 55, listOfTargetsDrone0, 0, 6, criticalDistance);
	Drone *drone4 = new Drone(500, 1, 55, listOfTargetsDrone0, 0, 4, criticalDistance);

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
	int i, j, tagIterator;

	// Erstellung der Threads
	static const int num_threads = 6;
	// t ist der Threads-Vektor
	std::thread t[num_threads];
	for (int i = 0; i < num_threads; ++i)
	{
		t[i] = std::thread(mape, &listOfDrones, i + 1);
	}

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
		//for-Schleife zur Klassifizierung der Drohnen in Gebieten
		for (it = listOfDrones.begin(); it != listOfDrones.end(); it++)
		{
			tagIterator = 1;
			for (i = 0; i < rows; i++)
			{
				for (j = 0; j < cols; j++)
				{
					if (it->getPosition().left > windowHStep * j && it->getPosition().left < windowHStep * (j + 1)
						&& it->getPosition().top > windowVStep * i && it->getPosition().top < windowVStep * (i + 1))
					{
						it->setAreaTag(tagIterator);
						// std::cout << "Drone " << it->getWeight() << " at " << tagIterator << std::endl;
						break;
					}
					// Inkrementierung der tagIterator
					tagIterator++;
				}
			}
		}

		// Clear everything from the last frame

		window.clear(Color(Color::Black));

		// Grenzen der Gebieten zeichnen
		// Horizontale Linien
		for (i = 1; i < rows; i++)
		{

			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(0, i * windowVStep)),
				sf::Vertex(sf::Vector2f(windowWidth, i * windowVStep))
			};
			window.draw(line, 2, sf::Lines);
		}
		// Vertikale Linien
		for (j = 1; j < cols; j++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(j * windowHStep, 0)),
				sf::Vertex(sf::Vector2f(j * windowHStep, windowHeight))
			};

			window.draw(line, 2, sf::Lines);
		}
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
	for (int i = 0; i < num_threads; ++i)
	{
		t[i].join();
	}

	return 0;
}