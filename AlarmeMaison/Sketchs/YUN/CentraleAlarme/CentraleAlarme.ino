#include <Process.h>

#define pinCapteurVeranda 2
#define pinCapteurPorteGarage 3
#define pinCapteurPorteJardin 4

#define ledDebug 13

#define VERANDA_OUVERTE 1
#define VERANDA_FERMEE 0

#define GARAGE_OUVERT 1
#define GARAGE_FERME 0

#define JARDIN_OUVERT 1
#define JARDIN_FERME 0

int etatCourantVeranda = VERANDA_FERMEE;
int etatPrecedentVeranda = VERANDA_FERMEE;

int etatCourantGarage = GARAGE_FERME;
int etatPrecedentGarage = GARAGE_FERME;

int etatCourantJardin = JARDIN_FERME;
int etatPrecedentJardin = JARDIN_FERME;

// initialisation du soft
void setup()
{
  // Initialisation du port série pour avoir un retour sur le serial monitor
  Serial.begin(9600); 
  // initialisation de la communication avec la partie Linux
  Bridge.begin();
  
  // Initialisation de l'entree du capteur d'ouverture en pull up
  pinMode(pinCapteurVeranda, INPUT_PULLUP);
  pinMode(pinCapteurPorteGarage, INPUT_PULLUP);
  pinMode(pinCapteurPorteJardin, INPUT_PULLUP);
  
   // utilisation de la led de debug
  digitalWrite(ledDebug, etatCourantVeranda);
}

// boucle principale
void loop()
{
  // Le changement d'etat de la porte est une opposition entre etatCourantVeranda et etatPrecedentVeranda
  // etatCourantVeranda = VERANDA_OUVERTE et etatPrecedentVeranda = VERANDA_FERMEE alors on vient d'ouvrir la porte
  // etatPrecedentVeranda = VERANDA_OUVERTE et  etatCourantVeranda = VERANDA_FERMEE alors on vient de fermer la porte
  
  etatCourantVeranda = digitalRead(pinCapteurVeranda);
  etatCourantJardin = digitalRead(pinCapteurPorteGarage);
  etatCourantGarage = digitalRead(pinCapteurPorteJardin);
  
  // ouverture de la porte, on envoie le mail
  if(etatCourantVeranda == VERANDA_OUVERTE && etatPrecedentVeranda == VERANDA_FERMEE)
  {
      GererOuvertureVeranda();
      return;
  }

//  // fermeture de la porte
//  if(etatCourantVeranda == VERANDA_FERMEE && etatPrecedentVeranda == VERANDA_OUVERTE)
//    etatPrecedentVeranda = VERANDA_FERMEE;

  digitalWrite(ledDebug, LOW);
}

// Traitement de l'ouverture de la porte de la veranda
void GererOuvertureVeranda()
{
  // utilisation de la led de debug pour afficher l etat du capteur
  digitalWrite(ledDebug, etatCourantVeranda);
  
  Process p;
  // Lancement du script python
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sda1/arduino/Alarme.py");
  p.run();
  // je considère la porte ouverte lorsque le mail a été envoyé
  etatPrecedentVeranda = digitalRead(pinCapteurVeranda);
  
  FaireClignoterLaLEDDeDebug();
}

void FaireClignoterLaLEDDeDebug()
{
  int etatLED = HIGH;
  
  for(int index = 0; index < 5; index++)
  {
    // clignotement de debug pour dire que le mail est parti
    digitalWrite(ledDebug, !etatLED);
    delay(500);
  }
}
