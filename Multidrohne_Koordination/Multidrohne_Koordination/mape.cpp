#include "stdafx.h"
#include "mape.h"

// Prototyp der Funktion für die Abstandsberechnung
float calcDist(vector<Drone>& listOfDrones, int droneI, int droneJ);
// Funktion für die Suche von all den Drohnen in der Nähe der ausgewählten
void findNearDrones(vector<Drone> &listOfDrones, int &nearDrone, int &nD, int &gw, bool *pPairFound);
// Funktion für die Suche von all den "verwandten" Drohnen; d.h. Drohnen, die nicht nahe dem Gruppenleiter stehen aber ja von den aktuellen Mitgliedern
void findRelativeDrones(vector<Drone> &listOfDrones, int &groupLeader, int &nearDrone, int &nD, int &gw, bool *pPairFound);
// Diese Funktion hier überprüft, ob zwei Vektoren die gleichen Komponenten haben
bool compareVectors(vector<int> &pa, vector<int> &pb);



// Definition des MAPE-Threads
void mape(vector<Drone> *pListOfDrones)
{
	// Deklaration Counter
	int i, j, n;
	// Bemerkung: Diese Parametern werden hier zum zweiten Mal deklariert
	int ndRef = 5;
	int gwRef = 200;
	//Deklaration des Vektor-Iterators
	vector<Drone>::iterator it;
	vector<Drone>::iterator pListStart = pListOfDrones->begin();
	// Parametern für die Weiterentwicklung des Monitorings
	bool pairFound;
	vector<vector <int>> listOfFinalGroups;
	vector<vector <int>>::iterator itFinal;
	bool finalTrigger;
	// Ich deklariere diese Variablen weil, sonst ergibt sich beim Aufrufen der compareVectors-Funktion
	// folgender Fehler : C++ initial value of reference to non-const must be an lvalue
	vector<int> a;
	vector<int> b;
	vector<int>::iterator itPrint;
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

		// Beim Start dieser Phase, löschen wir all die Gruppen
		for (i = 0; i < ndRef; i++)
		{
			(pListStart + i)->eraseGroup();
		}
		// Wir löschen die endgültigen Gruppen auch
		listOfFinalGroups.erase(listOfFinalGroups.begin(), listOfFinalGroups.end());
		// Weiterentwicklung des Monitorings
		for (i = 0; i < ndRef; i++)
		{
			// Sucht & Hinzufügt nahe Drohnen
			findNearDrones(*pListOfDrones, i, ndRef, gwRef, &pairFound);
			if (pairFound == true)
			// Wir suchen jetzt aktiv nach "verwandten" Gruppenmitglieder, d.h. Drohnen die nah von den aktuellen Mitgliedern stehen
			for (j = 1; j < (pListStart + i)->getGroup().size(); j++)
			{
				findRelativeDrones(*pListOfDrones, i, (pListStart + i)->getGroup().at(j), ndRef, gwRef, &pairFound);
				if (pairFound == true)
				{
					// Wenn diese Bedingung gestimmt hat, starten wir die Schleife neu
					j = 1;
				}
			}
		}

		
		// Wir führen jetzt eine Auswahl der endgültigen Gruppen durch
		for (i = 0; i != ndRef - 1; ++i)
		{
			for (j = 1; j != ndRef; ++j)
			{
				// Wir haben zuerst eine Auswahl aller möglichen Paaren Drohnen gemacht
				if (j > i)
				{
					a = (pListStart + i)->getGroup();
					b = (pListStart + j)->getGroup();
					// 1: Zahl Komponenten gleich?
					// 2: Befindet sich jede Komponente eines Vektors auch in dem anderen?
					if ((pListStart + i)->getGroup().size() == (pListStart + j)->getGroup().size() && compareVectors(a, b))
					{
						// Jetzt überprüfen wir, dass diese endgultige Gruppe nicht bereits in der Liste von aktuellen, endgültigen Gruppen stand
						finalTrigger = true;
						for (itFinal = listOfFinalGroups.begin(); itFinal != listOfFinalGroups.end(); itFinal++)
						{
							if (compareVectors(a, *itFinal))
							{
								// Wenn wir detektieren, die Gruppe war schon da, stellen wir sofort den Trigger auf false
								finalTrigger = false;
							}
						}
						// Wenn der Trigger nach der Überprüfung noch auf true ist, bedeutet das, wir müssen die neue Gruppe hinzufügen
						if (finalTrigger)
						{
							listOfFinalGroups.push_back(a);
						}

					}
				}
			}
		}

		/*for (i = 0; i < ndRef; i++)
		{
			(pListStart + i)->printGroup();
		}*/

		i = 0;
		std::cout << "\nNumber of final groups:" << listOfFinalGroups.size() << std::endl;
		for (itFinal = listOfFinalGroups.begin(); itFinal != listOfFinalGroups.end(); itFinal++)
		{
			
			std::cout << "\nFinal group" << i << "\t";
			for (itPrint = itFinal->begin(); itPrint != itFinal->end(); itPrint++)
			{
				std::cout << *itPrint << "\t";
			}
			i++;
		}
		// for-Schleife für die Durchfürung des Überprüfungsvorgang
		// BEMERKUNG: Das ist eine andere Weg, eine Schleife zum Bearbeitung einer vector_Variable vorzunehmen aber nur mithilfe dem pListStart-Iterator. 
	/*	for (i = 0; i != ndRef - 1; ++i)
		{
			for (j = 1; j != ndRef; ++j)
			{
				if (j > i)
				{
					if (calcDist(*pListOfDrones, i, j) < gwRef)
					{
						(pListStart + i)->setID((pListStart + i)->getID() + 1);
						(pListStart + j)->setID((pListStart + j)->getID() + 1);

					}
				}
			}
		}*/


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
			*/
			if ((pListStart + i)->getID() && (pListStart + i)->getTargetZActive() == false)
			{
				(pListStart + i)->setGoalViolated(true);
				(pListStart + i)->setColorGreen();
			}
			else if ((pListStart + i)->getID() == 0)
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
			if ((pListStart + i)->getGoalViolated() && (pListStart + i)->getTargetZActive() == false)
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
			for (i = 0; i < nc; i++)
			{
				*(corrVectStart + i) = 0;
			}

			// Aufbau von corrVect (Vector of corridors) und Zuweisung des aktuellen Korridors zu jeder Drohne (in der Klasse)
			for (it = pListOfDrones->begin(); it != pListOfDrones->end(); it++)
			{
				if (it->getGoalViolated() && it->getTargetZActive() == false)
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
			std::cout << "\n" << std::endl;
		
			for (i = nc - 1; i >= 0; i--)
			{
				std::cout << "Corridor " << i << " : " << *(corrVectStart + i) << std::endl;
				if (*(corrVectStart + i) > 1) // -> Das heißt, Korridor überbelegt
				{
					freeCorrFlag = false;	
					// Hier geben wir einen großen Wert ein (damit jeder andere Gewicht kleiner sein kann)
					minWeight = 10;
					// Leichteste Drohne im Korridor finden
					for (n = 0; n != ndRef; n++)
					{
						// Wir prüfen, ob diese Drohne im aktuellen Korridor liegt und gleichzeitig, ob diese die leichteste da ist
						if ((pListStart + n)->getCurrCorr() == i && (pListStart + n)->getWeight() < minWeight && (pListStart + n)->getGoalViolated())
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
			if ((pListStart + i)->getTargetZActive())
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
			(pListStart+i)->setID(0);
		}

		// Wartedauer des MAPE-K Loops
		Sleep(50);
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


void findNearDrones(vector<Drone> &listOfDrones, int &nearDrone, int &nD, int &gw, bool *pPairFound)
{
	vector<Drone>::iterator pListStart = listOfDrones.begin();
	*pPairFound = false;
	for (int n = 0; n < nD; n++)
	{
		// 1. Der Abstand wird überschritten 2. Die Drohne vergleichet sich nicht mit ihr selbst
		if (calcDist(listOfDrones, nearDrone, n) < gw && n != nearDrone && (pListStart + nearDrone)->findDroneInGroup(n) == false)
		{
			//std::cout << "Pair for " << nearDrone << "  found: " << n <<std::endl;
			*pPairFound = true;
			(pListStart + nearDrone)->addDroneToGroup(n);
		}
	}
}

void findRelativeDrones(vector<Drone> &listOfDrones, int &groupLeader, int &nearDrone, int &nD, int &gw, bool *pPairFound)
{
	vector<Drone>::iterator pListStart = listOfDrones.begin();
	*pPairFound = false;
	for (int n = 0; n < nD; n++)
	{
		// 1. Der Abstand wird überschritten 
		//2. Die Drohne vergleichet sich nicht mit ihr selbst 
		//3. Die gefundene Drohne steht nicht bereits in der Gruppe
		// Hier liegt der erste Unterschied zwischen beiden Funktionen: die 3.Bedingung wird für der Gruppe des Gruppenleiters überprüft
		if (calcDist(listOfDrones, nearDrone, n) < gw && n != nearDrone && (pListStart + groupLeader)->findDroneInGroup(n) == false)
		{
			//std::cout << "Pair for " << nearDrone << "  found: " << n << std::endl;
			*pPairFound = true;
			// Hier liegt der zweite Unterschied zwischen beiden Funktionen: die Zuweisung wird jetzt der Gruppe des Gruppenleiters gemacht
			(pListStart + groupLeader)->addDroneToGroup(n);
		}
	}
}

bool compareVectors(vector<int> &pa, vector<int> &pb)
{
	vector<int>::iterator ita;
	vector<int>::iterator itb;
	bool compFound;
	for (ita = pa.begin(); ita != pa.end(); ita++)
	{
		compFound = false;
		for (itb = pb.begin(); itb != pb.end(); itb++)
		{
			if (*ita == *itb)
			{
				compFound = true;
				break;
			}
		}
		if (compFound == false)
		{
			return false;
		}
	}
	return true;
}