#include <Process.h>

#define pinCapteurPorteVeranda 5
#define pinCapteurPorteGarage 3
#define pinCapteurPorteJardin 4

#define ledStatutEnvoiMail 13
#define ledEtatPorteVeranda 12
#define ledEtatPorteGarage 11
#define ledEtatPorteJardin 10


#define VERANDA_OUVERTE 1
#define VERANDA_FERMEE 0

#define GARAGE_OUVERT 1
#define GARAGE_FERME 0

#define JARDIN_OUVERT 1
#define JARDIN_FERME 0

int etatCourantPorteVeranda = VERANDA_FERMEE;
int etatPrecedentPorteVeranda = VERANDA_FERMEE;

int etatCourantPorteGarage = GARAGE_FERME;
int etatPrecedentPorteGarage = GARAGE_FERME;

int etatCourantPorteJardin = JARDIN_FERME;
int etatPrecedentPorteJardin = JARDIN_FERME;

// initialisation du soft
void setup()
{
  // Initialisation du port série pour avoir un retour sur le serial monitor
  Serial.begin(9600); 
  // initialisation de la communication avec la partie Linux
  Bridge.begin();
  
  // Initialisation de l'entree des capteurs d'ouverture en pull up
  pinMode(pinCapteurPorteVeranda, INPUT_PULLUP);
  pinMode(pinCapteurPorteGarage, INPUT_PULLUP);
  pinMode(pinCapteurPorteJardin, INPUT_PULLUP);
  
  pinMode(ledEtatPorteVeranda, OUTPUT);
  pinMode(ledEtatPorteGarage, OUTPUT);
  pinMode(ledEtatPorteJardin, OUTPUT);


}

// boucle principale
void loop()
{
  // Le changement d'etat de la porte est une opposition entre etatCourant et etatPrecedent
  // etatCourant = OUVERT et etatPrecedent = FERME alors on vient d'ouvrir la porte
  // etatPrecedent = OUVERT et  etatCourant = FERME alors on vient de fermer la porte
  
  etatCourantPorteVeranda = digitalRead(pinCapteurPorteVeranda);
  etatCourantPorteGarage = digitalRead(pinCapteurPorteGarage);
  etatCourantPorteJardin = digitalRead(pinCapteurPorteJardin);

  Serial.println("Veranda : " + String(etatCourantPorteVeranda) + " - Garage : " + String(etatCourantPorteGarage) + " - Jardin : " + String(etatCourantPorteJardin));
  
  // utilisation de la led de debug pour afficher l etat du capteur
  digitalWrite(ledEtatPorteVeranda, etatCourantPorteVeranda);
  digitalWrite(ledEtatPorteGarage, etatCourantPorteGarage);
  digitalWrite(ledEtatPorteJardin, etatCourantPorteJardin);

  // Gestion de l'envoi des mails
  ScruterEtatVeranda();
  ScruterEtatGarage();
  ScruterEtatJardin();
}

void ScruterEtatVeranda()
{
	if (etatCourantPorteVeranda == VERANDA_OUVERTE && etatPrecedentPorteVeranda == VERANDA_FERMEE)
	{
		GererOuvertureVeranda();
		return;
	}

	if (etatCourantPorteVeranda != etatPrecedentPorteVeranda)
		etatPrecedentPorteVeranda = etatCourantPorteVeranda;
}

// Traitement de l'ouverture de la porte de la veranda
void GererOuvertureVeranda()
{
  Process p;
  // Lancement du script python
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sda1/arduino/EnvoyerMailSurOuverturePorteVeranda.py");
  p.run();
  // je considère la porte ouverte lorsque le mail a été envoyé
  etatPrecedentPorteVeranda = digitalRead(pinCapteurPorteVeranda);
  
  FaireClignoterLaLEDDeDebug();
}

void ScruterEtatGarage()
{
	if (etatCourantPorteGarage == GARAGE_OUVERT && etatPrecedentPorteGarage == GARAGE_FERME)
	{
		GererOuvertureGarage();
		return;
	}

	if (etatCourantPorteGarage != etatPrecedentPorteGarage)
		etatPrecedentPorteGarage = etatCourantPorteGarage;
}

void GererOuvertureGarage()
{
  Process p;
  // Lancement du script python
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sda1/arduino/EnvoyerMailSurOuverturePorteGarage.py");
  p.run();
	etatPrecedentPorteGarage = digitalRead(pinCapteurPorteGarage);

  FaireClignoterLaLEDDeDebug();
}

void ScruterEtatJardin()
{
	if (etatCourantPorteJardin == JARDIN_OUVERT && etatPrecedentPorteJardin == JARDIN_FERME)
	{
		GererOuvertureJardin();
		return;
	}

	if (etatCourantPorteJardin != etatPrecedentPorteJardin)
		etatPrecedentPorteJardin = etatCourantPorteJardin;
}

void GererOuvertureJardin()
{
  Process p;
  // Lancement du script python
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sda1/arduino/EnvoyerMailSurOuverturePorteJardin.py");
  p.run();
	etatPrecedentPorteJardin = digitalRead(pinCapteurPorteJardin);

  FaireClignoterLaLEDDeDebug();
}

void FaireClignoterLaLEDDeDebug()
{
  int etatLED = HIGH;
  
  for(int index = 0; index < 5; index++)
  {
    // clignotement de debug pour dire que le mail est parti
    digitalWrite(ledStatutEnvoiMail, !etatLED);
    delay(500);
  }
}
