#include <Process.h>

#define pinCapteurVeranda 2

int etat;
int etatPrecedent = LOW;

// initialisation du soft
void setup()
{
  Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
  Bridge.begin();
  // Initialisation de l'entree du capteur d'ouverture en pull up
  pinMode(pinCapteurVeranda, INPUT_PULLUP);
  // lecture de l'etat actuel du capteur
  etat = digitalRead(pinCapteurVeranda);
  // on considere que l'etatPrecedent est l'inverse de l'etat courant
  etatPrecedent = !etat;
  // utilisation de la led de debug
  digitalWrite(13, etat);
}

// boucle principale
void loop()
{
  etat = digitalRead(pinCapteurVeranda);
  
  // si on est à l'etat haut (porte ouverte) et que ce n'est pas l'etat precedent
  // pour gerer le cas ou la porte reste ouverte, on envoie une seule fois le mail
  if(etat == HIGH && etat != etatPrecedent)
    GererOuvertureVeranda();

  etatPrecedent = etat;
  digitalWrite(13, LOW);
}

// Traitement de l'ouverture de la porte de la veranda
void GererOuvertureVeranda()
{
  // utilisation de la led de debug
  digitalWrite(13, etat);
  
  Process p;
  // Lancement du script python
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sda1/arduino/Alarme.py");
  p.run();
  
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
    delay(500);
    
    

}
