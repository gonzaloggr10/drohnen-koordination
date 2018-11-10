#include "stdafx.h"
#include "Drone.h"

Drone::Drone(float startX, float startY, float startZ, vector<Vector2f> tl, int ID, int intWeight) 
{
	position.x = startX; // Diese Position entspricht der oberen, linken Ecke
	position.y = startY;
	z = startZ;
	currTarget = tl[0];
	listOfTargets = tl;
	id = ID;
	weight = intWeight;	
	flag = 1;
	goalViolated = false;

	// Wir integrieren die erste Drohne in ihre Gruppe
	group.push_back(weight);

	// Etablierung der Grenzwert
	gw = 200;

	// Wir definieren die entsprechende Geschwindigkeiten dem ersten Ziel zufolge
	xVelocity = calcVelocity(currTarget).x;
	yVelocity = calcVelocity(currTarget).y;

	// Mit den Eingabedaten definieren wir den "RectangleShape" Objekt an der Stelle
	droneShape.setSize(Vector2f(90 * z / 100, 90 * z / 100)); // 90 ist die maximale Gr��e und 100 ist die Normalisierungskoeffizient
	droneShape.setPosition(position);
	// Wir angeben manche Einstellungen f�r die Darstellung der Auren
	positionAura = position;
	positionAura.x -= gw;
	positionAura.y -= gw;
	droneAura.setRadius(gw);
	droneAura.setFillColor(Color(Color::Transparent));
	droneAura.setOutlineColor(sf::Color::Blue);
	droneAura.setOutlineThickness(0.2);
	droneAura.setPosition(positionAura);
	// N�tige Elemente f�r die Anzeige des Textes
	// Damit den Text oben auf den Drohnen angezeigt wird, machen wir extra eine Variable dazu
	positionText = position;
	positionText.y -= 25;
	font.loadFromFile("RockoFLF.ttf");
	droneText.setFont(font);
	//droneText.setString("120");
	droneText.setCharacterSize(20);
	droneText.setFillColor(sf::Color::Yellow);
	//droneText.setPosition(positionText);

	// N�tige Elemente f�r die Anzeige des Textes
	// Damit den Text oben auf den Drohnen angezeigt wird, machen wir extra eine Variable dazu
	positionWeight = position;
	positionWeight.y += 25;
	font.loadFromFile("RockoFLF.ttf");
	droneWeight.setFont(font);
	droneWeight.setCharacterSize(12);
	droneWeight.setFillColor(sf::Color::Yellow);
}
FloatRect Drone::getPosition()
{
	return droneShape.getGlobalBounds();
}
RectangleShape Drone::getShape()
{
	return droneShape;
}
CircleShape Drone::getDroneAura()
{
	return droneAura;
}
Text Drone::getText()
{
	return droneText;
}
Text Drone::getDroneWeight()
{
	return droneWeight;
}
void Drone::addDroneToGroup(int droneCoef)
{
	//std::cout << "Drone " << droneCoef << "added to group " << weight << std::endl;
	group.push_back(droneCoef);
}
vector<int> Drone::getGroup()
{
	return group;
}
void Drone::printGroup()
{
	std::cout << "\nComponents of group " << weight << " are " ;
	for (itG = group.begin();itG != group.end(); itG++)
	{
		std::cout << *itG << "\t";
	}
}
void Drone::eraseGroup()
{
	// Darauf achten, dass die erste Komponente der Gruppenleiter ist und deswegen musst sie nicht gel�scht werden
	group.erase(group.begin() + 1, group.end());
	/*std::cout << "\nGroup after erase  for Drone " << weight << " is ";
	for (itG = group.begin();itG != group.end(); itG++)
	{
		std::cout << *itG << "\t";
	}*/
}
void Drone::setGroupTag(int & gTag)
{
	// Wenn die Drohne zu keiner Gruppe geh�rt, weisen wir ihr die Zahl -1 zu
	groupTag = gTag;
}
int Drone::getGroupTag()
{
	return groupTag;
}
bool Drone::findDroneInGroup(int drone)
{
	for (itG = group.begin();itG != group.end(); itG++)
	{
		if (drone == *itG)
		{
			return true;
		}
	}
	return false;
}
void Drone::setColorBlue()
{
	droneShape.setFillColor(Color(Color::Blue));
}
void Drone::setColorWhite()
{
	droneShape.setFillColor(Color(Color::White));
}
void Drone::setZGoal(float zSetGoal)
{
	zGoal = zSetGoal;
	// Wenn ein Ziel f�r die Drohne zugewiesen wird, bewegt sich die Drohne entlang die Z-Achse solange targetZActive auf "true" gesetzt ist
	targetZActive = true;
	Drone::setZVelocity();
}
bool Drone::getTargetZActive()
{
	return targetZActive;
}
void Drone::setTargetZActive(bool ztarget)
{
	targetZActive = ztarget;
}
void Drone::setZVelocity()
{
	if (zGoal > z)
	{
		zVelocity = .5f;
	}
	else if (zGoal < z)
	{
		zVelocity = -.5f;
	}
}
bool Drone::getExecuteActive()
{
	return executeActive;
}
void Drone::setExecuteActive(bool executeValue)
{
	executeActive = executeValue;
}
float Drone::getZ()
{
	return z;
}
float Drone::getXVelocity()
{
	return xVelocity;
}
int Drone::getWeight()
{
	return this->weight;
}
bool Drone::getGoalViolated()
{
	return this->goalViolated;
}
void Drone::setGoalViolated(bool newValue)
{
	goalViolated = newValue;
}
int Drone::getCurrCorr()
{
	return this->currCorr;
}
void Drone::setCurrCorr(int CurrCorr)
{
	currCorr = CurrCorr;
}
void Drone::setID(int idsetter)
{
	this->id = idsetter;
}
int Drone::getID()
{
	return this->id;
}
Vector2f Drone::calcVelocity(Vector2f &t)
{
	Vector2f nVelocity;

	// Wir f�hren hier die folgende Berechnung: (p1-p0) / |p1 - p0| bzw. Abzug und Normalisation der Vektoren
	nVelocity = t - position;
	nVelocity = nVelocity / sqrt(pow(nVelocity.x, 2) + pow(nVelocity.y, 2)) * .2f;
	return nVelocity;

}
void Drone::reboundSides()
{
	xVelocity = - xVelocity;
}
void Drone::reboundBottomOrTop()
{
	yVelocity = - yVelocity;
}
void Drone::update()
{
	// Updatet die Positionvariablen der Drohne
	position.x += xVelocity;
	position.y += yVelocity;

	positionText = position;
	positionText.y -= 25;

	positionWeight = position;
	positionWeight.y += 25;

	positionAura = position;
	positionAura.x -= gw;
	positionAura.y -= gw;
	// Wenn ein Ziel f�r die Drohne zugewiesen wird, bewegt sich die Drohne entlang die Z-Achse solange targetZActive auf "true" gesetzt ist
	if(executeActive)
	{
		z += zVelocity;
		if (z > zGoal - 1 && z < zGoal + 1)
		{
			executeActive = false;
			targetZActive = false;
		}
	}
	
	

	// Bedingung: Ziel erreicht: Die Drohne steht innerhalb die definierten Grenzen in den x- und y-Achsen
	if (position.x > currTarget.x - 30 && position.x < currTarget.x + 30 && position.y > currTarget.y - 30 && position.y < currTarget.y + 30)
	{
		// Wenn die Drohne das aktuelle Ziel erreicht, wird das Ziel auf der Stelle zum n�chsten Ziel gewechselt ( wenn ein n�chstes noch gibt)
		// Bemerkung: Diese Struktur k�nnte mit einer for-Schleife umgetauscht werden, wenn die breaks Ged�chtnis h�tten und dann, als die Schleife
		// noch mal zugriffen wird, w�rde es auf der letzten Stelle wieder angenommen

		// Initialisierung des Z�hlers f�r die Schleifen an "listOfTargets". Ich mache sowas, weil ich den Iterator im Konstruktor nicht initialisieren
		// kann ( itT = listOfTargets.begin();). Es ergibt sich folgenden Fehler: "vector iterators incompatible"
		if (flag)
		{
			itT = listOfTargets.begin();
			flag = 0;
		}
		// Solche Methode funktioniert, weil, sobald die Drohne die Grenze �berschreitet, 
		if (itT != listOfTargets.end())
		{
			currTarget = *itT;
			// Aktualisierung der Geschwindigeit
			xVelocity = calcVelocity(currTarget).x;
			yVelocity = calcVelocity(currTarget).y;
			// Das funktioniert anders als gew�hnlich: hier wird die Variable inkrementiert, damit wir diesen neuen Wert bei der n�chste Runde 
			// benutzen k�nnen
			itT++;
		}
		else
		{
			xVelocity = 0;
			yVelocity = 0;
		}
	}

	// Bewegt die Drohne
	droneShape.setPosition(position);
	droneShape.setSize(Vector2f(25 * z / 100, 25 * z / 100)); // 25 ist die maximale Gr��e und 100 ist die Normalisierungskoeffizient
	// Bewegt die Aura
	droneAura.setPosition(positionAura);
	// Bewegt den Text
	droneText.setPosition(positionText);
	// Aktualisiert den Text
	heightString = to_string(int(z));
	droneText.setString(heightString);
	// Bewegt den Gewicht
	droneWeight.setPosition(positionWeight);
	// Aktualisiert den Gewicht
	weightString = to_string(int(weight));
	weightString = string("Drone ") + weightString;
	droneWeight.setString(weightString);
}