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
	

	// Wir definieren die entsprechende Geschwindigkeiten dem ersten Ziel zufolge
	xVelocity = calcVelocity(currTarget).x;
	yVelocity = calcVelocity(currTarget).y;

	// Mit den Eingabedaten definieren wir den "RectangleShape" Objekt an der Stelle
	droneShape.setSize(Vector2f(90 * z / 100, 90 * z / 100)); // 90 ist die maximale Gr��e und 100 ist die Normalisierungskoeffizient
	droneShape.setPosition(position);
	// N�tige Elemente f�r die Anzeige des Textes
	// Damit den Text oben auf den Drohnen angezeigt wird, machen wir extra eine Variable dazu
	positionText = position;
	positionText.y -= 25;
	font.loadFromFile("Wedgie Regular.ttf");
	droneText.setFont(font);
	droneText.setString("120");
	droneText.setCharacterSize(20);
	//droneText.setStyle(sf::Text::Bold);
	droneText.setFillColor(sf::Color::Yellow);
	droneText.setPosition(position);
}
FloatRect Drone::getPosition()
{
	return droneShape.getGlobalBounds();
}
RectangleShape Drone::getShape()
{
	return droneShape;
}
Text Drone::getText()
{
	return droneText;
}
void Drone::setColorGreen()
{
	droneShape.setFillColor(Color(Color::Green));
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

		if (itT != listOfTargets.end())
		{
			currTarget = *itT;
			// Aktualisierung der Geschwindigeit
			xVelocity = calcVelocity(currTarget).x;
			yVelocity = calcVelocity(currTarget).y;
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
	droneShape.setSize(Vector2f(80 * z / 100, 80 * z / 100)); // 80 ist die maximale Gr��e und 100 ist die Normalisierungskoeffizient
	// Bewegt den Text
	droneText.setPosition(positionText);
	// Aktualisiert den Text
	heightString = to_string(int(z));
	droneText.setString(heightString);
}