#include <Process.h>

#define pinCapteurVeranda 2
#define ledDebug 13
#define OUVERT 1
#define FERME 0

int etat = FERME;
int etatPrecedent = FERME;

// initialisation du soft
void setup()
{
  // Initialisation du port série pour avoir un retour sur le serial monitor
  Serial.begin(9600); 
  // initialisation de la communication avec la partie Linux
  Bridge.begin();
  
  // Initialisation de l'entree du capteur d'ouverture en pull up
  pinMode(pinCapteurVeranda, INPUT_PULLUP);
  
   // utilisation de la led de debug
  digitalWrite(ledDebug, etat);
}

// boucle principale
void loop()
{
  // Le changement d'etat de la porte est une opposition entre etat et etatPrecedent
  // etat = OUVERT et etatPrecedent = FERME alors on vient d'ouvrir la porte
  // etatPrecedent = OUVERT et  etat = FERME alors on vient de fermer la porte
  
  etat = digitalRead(pinCapteurVeranda);
  
  // ouverture de la porte, on envoie le mail
  if(etat == OUVERT && etatPrecedent == FERME)
  {
      GererOuvertureVeranda();
      return;
  }

  // fermeture de la porte
  if(etat == FERME && etatPrecedent == OUVERT)
    etatPrecedent = FERME;

  digitalWrite(ledDebug, LOW);
}

// Traitement de l'ouverture de la porte de la veranda
void GererOuvertureVeranda()
{
  // utilisation de la led de debug pour afficher l etat du capteur
  digitalWrite(ledDebug, etat);
  
  Process p;
  // Lancement du script python
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sda1/arduino/Alarme.py");
  p.run();

  FaireClignoterLaLEDDeDebug();
  etatPrecedent = OUVERT;
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


