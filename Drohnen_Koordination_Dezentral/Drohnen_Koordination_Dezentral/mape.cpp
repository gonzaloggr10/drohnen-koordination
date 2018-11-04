#include "stdafx.h"
#include "mape.h"

// Prototyp der Funktion für die Abstandsberechnung
float calcDist(vector<Drone>& listOfDrones, int droneI, int droneJ);

// Definition des MAPE-Threads
void mape(vector<Drone> *pListOfDrones, int aTag)
{
	// Definition der Variable für's Gebiet-Tag
	// Deklaration Counter
	int i, j, n;
	// Bemerkung: Diese Parametern werden hier zum zweiten Mal deklariert
	int ndRef = 5;
	int cDMape = 200;
	//Deklaration des Vektor-Iterators
	vector<Drone>::iterator it;
	vector<Drone>::iterator pListStart = pListOfDrones->begin();
	/*
	Deklaration von Parametern für die Drohnenanordnungs-Methode
	*********************************************************************
	*/
	bool triggerPlan;
	int corrHeight = 10;
	int minWeight;
	int	lightestDrone;
	bool freeCorrFlag = false;
	// Folgend steht die Definition der Variable "number of corridors". Die Formell hier hat die Form: (zMax - zMin) / corrHeight
	int nc = (100 - 50) / corrHeight;
	// corrVect: Vektor von Korridoren, der in jeder Komponente die Anzahl an Drohnen behaltet, die sich in jeweiligem Korridor befinden
	std::vector<int> corrVect(nc);
	std::vector<int>::iterator corrVectStart = corrVect.begin();
	/*
	*********************************************************************
	*/

	while (true)
	{
		/*
		MONITOR
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		// for-Schleife für die Durchfürung des Überprüfungsvorgang
		// BEMERKUNG: Das ist eine andere Weg, eine Schleife zum Bearbeitung einer vector_Variable vorzunehmen aber nur mithilfe dem pListStart-Iterator. 
		for (i = 0; i != ndRef - 1; ++i)
		{
			for (j = 1; j != ndRef; ++j)
			{
				// Wir überprüfen, dass sowohl die i-Drohne als auch die j-Drohne zu diesem Gebiet gehören
				if (j > i && (pListStart + i)->getAreaTag() == aTag && (pListStart + j)->getAreaTag() == aTag)
				{
					if (calcDist(*pListOfDrones, i, j) < cDMape)
					{
						(pListStart + i)->setID((pListStart + i)->getID() + 1);
						(pListStart + j)->setID((pListStart + j)->getID() + 1);

					}
				}
			}
		}

		/*
		ANALYSIS
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		//"Violated Goals" detektieren und goalViolated Variable auf "true" setzen
		for (i = 0; i != ndRef; ++i)
		{
			/*
			Bei dieser Bedingung wir überprufen:
			1. Der Abstand zwischen dieser Drohne und irgendanderen Drohne ist kleiner als erlaub
			2. Diese Drohne befindet sich nicht dabei, ihr Z-Ziel zu erreichen
			3. Die Drohne gehört zu diesem Gebiet
			*/
			if ((pListStart + i)->getID() && (pListStart + i)->getTargetZActive() == false && (pListStart + i)->getAreaTag() == aTag)
			{
				(pListStart + i)->setGoalViolated(true);
				(pListStart + i)->setColorGreen();
			}
			else if ((pListStart + i)->getID() == 0 && (pListStart + i)->getAreaTag() == aTag)
				// Bemerkung: Wir beziehen jetzt die zweite Bedingung nicht ein, da,  wenn das der Fall wäre, würden die Drohnen weiß, die
				// sich in der Z-Achse bewegen. Das hat dpch jetzt eine wichtige Folge: die "goalViolated" Flag wird in diesen Drohnen auf true
				// bleiben. Darauf werden wird im nächsten Tel aufpassen müssen -> (Oder, wir lassen die "goalViolated" Variable automatisch auf
				// false schalten, wenn "targetZActive" auf true ist) -> Nicht gemacht, um rigoros zu bleiben
			{
				(pListStart + i)->setGoalViolated(false);
				(pListStart + i)->setColorWhite();
			}
		}

		// Bemerkung: Das was wir jetzt machen ist doof: Wir wussten schon welche Drohnen hatten ein Ziel verletzt
		triggerPlan = false;
		for (i = 0; i != ndRef; ++i)
		{
			// Bei dieser Bedingung überprüfen wir noch mal dass die Drohne zu diesem Gebiet gehören
			if ((pListStart + i)->getGoalViolated() && (pListStart + i)->getTargetZActive() == false && (pListStart + i)->getAreaTag() == aTag)
			{
				triggerPlan = true;
			}
		}

		/*
		PLAN
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		// for-Schleife zum Zurücksetzen zu 0 des "corridor vector"s
		// Optimieren: Rauskriegen, wie man mit der vector-Klasse den Komponenten direkt Werte zuweist.

		if (triggerPlan)
		{
			// all die Komponenten von corrVect auf 0 setzen
			for (i = 0; i < nc; i++)
			{
				*(corrVectStart + i) = 0;
			}

			// Aufbau von corrVect (Vector of corridors) und Zuweisung des aktuellen Korridors zu jeder Drohne (in der Klasse)
			for (it = pListOfDrones->begin(); it != pListOfDrones->end(); it++)
			{
				// Bei dieser Bedingung überprüfen wir noch mal dass die Drohne zu diesem Gebiet gehören
				if (it->getGoalViolated() && it->getTargetZActive() == false && it->getAreaTag() == aTag)
				{
					for (i = 0; i < nc; i++)
					{
						// Bedingung der Zugehörigkeit zu Flugkorridor
						if (50 + i * corrHeight <= it->getZ() && it->getZ() < 50 + (i + 1) * corrHeight)
						{
							it->setCurrCorr(i);
							*(corrVectStart + i) = *(corrVectStart + i) + 1;
						}
					}
				}
			}
			// Drohnenanordnung-Methode
			// Bemerkung: Die Schleif geht von Oben nach Unten
			//std::cout << "\n" << std::endl;

			for (i = nc - 1; i >= 0; i--)
			{
				//std::cout << "Corridor " << i << " : " << *(corrVectStart + i) << std::endl;
				if (*(corrVectStart + i) > 1) // -> Das heißt, Korridor überbelegt
				{
					freeCorrFlag = false;
					// Hier geben wir einen großen Wert ein (damit jeder andere Gewicht kleiner sein kann)
					minWeight = 10;
					// Leichteste Drohne im Korridor finden
					for (n = 0; n != ndRef; n++)
					{
						// Wir prüfen, ob diese Drohne im aktuellen Korridor liegt und gleichzeitig, ob diese die leichteste da ist
						// Bei dieser Bedingung überprüfen wir noch mal dass die Drohne zu diesem Gebiet gehören
						if ((pListStart + n)->getCurrCorr() == i && (pListStart + n)->getWeight() < minWeight && (pListStart + n)->getGoalViolated() && (pListStart + n)->getAreaTag() == aTag)
						{
							std::cout << (pListStart + n)->getWeight() << " less than " << minWeight << std::endl;
							minWeight = (pListStart + n)->getWeight();
							lightestDrone = n;
							std::cout << "Lightest Drone found: " << n << std::endl;
						}
					}
					// "Rundschleife mit Umkehrung" zu dem Zweck, die Belegung aller Korridore zu überprufen und den Zielkorridor zuzuweisen
					// Erklärung dazu auf den Notizen
					for (n = (nc - 1) - ((i + nc - 1) % nc); n != (nc - 1) - i; n = (n + 1) % nc)
					{
						// Diese ist die Iterationsvariable, die wir schließlich benutzten (zu beachten ist, dabei wird eine zweite Umkehrung gemacht)
						// So, j ist mein Zielkorridor
						j = (nc - 1) - n;
						std::cout << "Iterator for the search of free corridor " << j << std::endl;
						if (*(corrVectStart + j) == 0 && freeCorrFlag == false)
						{
							// Jetzt nehmen wir eine Zuweisung der neuen Höhe für die leichteste Drohne vor
							(pListStart + lightestDrone)->setZGoal(50 + j * corrHeight + corrHeight / 2);
							std::cout << "Drone " << lightestDrone << " to corr" << j << std::endl;
							freeCorrFlag = true;
							// Wir müssen jetzt sowohl corrVect (Vector of corridors) als auch die Zuweisung des Korridors zur Drohne aktualisieren
							*(corrVectStart + i) = *(corrVectStart + i) - 1;
							*(corrVectStart + j) = *(corrVectStart + j) + 1;
							(pListStart + lightestDrone)->setCurrCorr(j);
						}
					}
					// Wir prüfen jetzt über, ob wir tatsächlich einen freien Korridor gefunden haben.
					if (freeCorrFlag == false)
					{
						std::cout << "Inminent Collision! All corridors are full" << std::endl;
					}
					// Jetzt, für den Fall, dass mehr als 2 Drohnen im Korridor lagen, überprüfen wir wieder ob wir das Verfahren wiederholen müssen
					if (*(corrVectStart + i) > 1)
					{
						std::cout << "Still " << *(corrVectStart + i) << " drones in corridor " << i << std::endl;
						i++;
					}
				}
			}
		}

		/*
		EXECUTION
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		// Hier geht's um einen zum Teil redundanten Schritt, der ermöglicht uns jedoch, die Planning & Exectution Phasen zu trennen
		for (i = 0; i != ndRef; ++i)
		{
			//std::cout << (pListStart + i)->getTargetZActive() << std::endl;
			// Bei dieser Bedingung überprüfen wir noch mal dass die Drohne zu diesem Gebiet gehören
			if ((pListStart + i)->getTargetZActive() && (pListStart + i)->getAreaTag() == aTag)
			{
				(pListStart + i)->setExecuteActive(true);
			}

		}

		/*std::cout << "\n\n" << std::endl;
		for (i = 0; i < nc; i++)
		{
		std::cout << *(corrVectStart + i) << std::endl;
		}
		*/
		//Identifikatoren zu 0 zurücksetzen
		for (i = 0; i != ndRef; ++i)
		{
			// Bei dieser Bedingung überprüfen wir noch mal dass die Drohne zu diesem Gebiet gehören
			if ((pListStart + i)->getAreaTag() == aTag)
			{
				(pListStart + i)->setID(0);
			}
		}

		// Wartedauer des MAPE-K Loops
		Sleep(20);
	}
}

// Definition der Funktion für die Abstandsberechnung
float calcDist(vector<Drone>& pListOfDrones, int droneI, int droneJ)
{
	Vector2f vect;	// Variable für den vektoriellen Abstand
	float dist;		// Rückgabe Variable
	vector<Drone>::iterator it;
	it = pListOfDrones.begin();
	// TODO: Zurzeit wird den Abstand zwischen den "top-left"-Punkten der beiden Rechtecken berechnet. Mehtode so modifizieren, dass einen Abstand
	// von Mittelpunkt zu Mittelpunkt berechnet wird.
	vect.x = (it + droneI)->getPosition().left - (it + droneJ)->getPosition().left;
	vect.y = (it + droneI)->getPosition().top - (it + droneJ)->getPosition().top;
	dist = sqrt(vect.x * vect.x + vect.y * vect.y);
	return dist;
}
