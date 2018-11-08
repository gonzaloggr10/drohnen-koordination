#include "stdafx.h"
#include "Drone.h"

Drone::Drone(float startX, float startY, float startZ, vector<Vector2f> tl, int ID, int intWeight, int critD)
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

	// Etablierung der Grenzwert
	cDDrone = critD;

	// Wir definieren die entsprechende Geschwindigkeiten dem ersten Ziel zufolge
	xVelocity = calcVelocity(currTarget).x;
	yVelocity = calcVelocity(currTarget).y;

	// Mit den Eingabedaten definieren wir den "RectangleShape" Objekt an der Stelle
	//droneShape.setSize(Vector2f(90 * z / 100, 90 * z / 100)); // 90 ist die maximale Größe und 100 ist die Normalisierungskoeffizient
	droneShape.setRadius(25 * z / 100); // 25 ist die maximale Größe und 100 ist die Normalisierungskoeffizient
	droneShape.setPosition(position);
	// Wir angeben manche Einstellungen für die Darstellung der Auren
	positionAura = position;
	positionAura.x -= cDDrone;
	positionAura.y -= cDDrone;
	droneAura.setRadius(cDDrone);
	droneAura.setFillColor(Color(Color::Transparent));
	droneAura.setOutlineColor(sf::Color::Blue);
	droneAura.setOutlineThickness(0.2);
	droneAura.setPosition(positionAura);
	// Nötige Elemente für die Anzeige des Textes
	// Damit den Text oben auf den Drohnen angezeigt wird, machen wir extra eine Variable dazu
	positionText = position;
	positionText.y -= 25;
	font.loadFromFile("RockoFLF.ttf");
	droneText.setFont(font);
	droneText.setString("120");
	droneText.setCharacterSize(20);
	droneText.setFillColor(sf::Color::Yellow);
	droneText.setPosition(position);
}
FloatRect Drone::getPosition()
{
	return droneShape.getGlobalBounds();
}
CircleShape Drone::getShape()
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
void Drone::setColorRed()
{
	droneShape.setFillColor(Color(Color::Red));
}
void Drone::setColorWhite()
{
	droneShape.setFillColor(Color(Color::White));
}
void Drone::setZGoal(float zSetGoal)
{
	zGoal = zSetGoal;
	// Wenn ein Ziel für die Drohne zugewiesen wird, bewegt sich die Drohne entlang die Z-Achse solange targetZActive auf "true" gesetzt ist
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
	// Wenn das Ziel als verletzt vorhanden is, setzten wir automatisch die Farbe auf Rot
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
int Drone::getAreaTag()
{
	return areaTag;
}
void Drone::setAreaTag(int &aTag)
{
	areaTag = aTag;
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

	// Wir führen hier die folgende Berechnung: (p1-p0) / |p1 - p0| bzw. Abzug und Normalisation der Vektoren
	nVelocity = t - position;
	nVelocity = nVelocity / sqrt(pow(nVelocity.x, 2) + pow(nVelocity.y, 2)) * .2f;
	return nVelocity;

}
void Drone::reboundSides()
{
	xVelocity = -xVelocity;
}
void Drone::reboundBottomOrTop()
{
	yVelocity = -yVelocity;
}
void Drone::update()
{
	// Updatet die Positionvariablen der Drohne
	position.x += xVelocity;
	position.y += yVelocity;

	positionText = position;
	positionText.y -= 25;

	positionAura = position;
	positionAura.x -= cDDrone;
	positionAura.y -= cDDrone;
	// Wenn ein Ziel für die Drohne zugewiesen wird, bewegt sich die Drohne entlang die Z-Achse solange targetZActive auf "true" gesetzt ist
	if (executeActive)
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
		// Wenn die Drohne das aktuelle Ziel erreicht, wird das Ziel auf der Stelle zum nächsten Ziel gewechselt ( wenn ein nächstes noch gibt)
		// Bemerkung: Diese Struktur könnte mit einer for-Schleife umgetauscht werden, wenn die breaks Gedächtnis hätten und dann, als die Schleife
		// noch mal zugriffen wird, würde es auf der letzten Stelle wieder angenommen

		// Initialisierung des Zählers für die Schleifen an "listOfTargets". Ich mache sowas, weil ich den Iterator im Konstruktor nicht initialisieren
		// kann ( itT = listOfTargets.begin();). Es ergibt sich folgenden Fehler: "vector iterators incompatible"
		if (flag)
		{
			itT = listOfTargets.begin();
			flag = 0;
		}
		// Solche Methode funktioniert, weil, sobald die Drohne die Grenze überschreitet, 
		if (itT != listOfTargets.end())
		{
			currTarget = *itT;
			// Aktualisierung der Geschwindigeit
			xVelocity = calcVelocity(currTarget).x;
			yVelocity = calcVelocity(currTarget).y;
			// Das funktioniert anders als gewöhnlich: hier wird die Variable inkrementiert, damit wir diesen neuen Wert bei der nächste Runde 
			// benutzen können
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
	// Aktualisiert die neue Größe der Drohne je nach der Höhe Z
	//droneShape.setSize(Vector2f(25 * z / 100, 25 * z / 100)); // 25 ist die maximale Größe und 100 ist die Normalisierungskoeffizient
	droneShape.setRadius(25 * z / 100); // 25 ist die maximale Größe und 100 ist die Normalisierungskoeffizient
															 
	// Bewegt die Aura
	droneAura.setPosition(positionAura);
	// Bewegt den Text
	droneText.setPosition(positionText);
	// Aktualisiert den Text
	heightString = to_string(int(z));
	droneText.setString(heightString);
}