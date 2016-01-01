#include <Process.h>

#define pinCapteurVeranda 2
#define OUVERT 1
#define FERME 0

int etat;
int etatPrecedent = LOW;
// necessaire car le capteur a 2 etats haut lors du glissé de l'aimant
int etatDeLaPorte;


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
  
  // on demarre porte fermee
  etatDeLaPorte = FERME;
  
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
  // Si l etat de la porte est deja ouvert, c'est qu'on est sur le second seuil haut du glissé, et c'est 
  // lors de la fermeture, dans ce cas on ignore. Le second seuil lors de l'ouverture est proche du premier
  // temporellement donc pas le temps de le détecter puisqu'on est en train de faire les acquisitions et l'envoi du mail
  if(etatDeLaPorte == OUVERT) 
  {
    etatDeLaPorte = FERME;
    return;
  }
  
  // la porte vient d etre ouverte 
  etatDeLaPorte = OUVERT;
  
  // utilisation de la led de debug pour afficher l etat du capteur
  digitalWrite(13, etat);
  
  Process p;
  // Lancement du script python
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sda1/arduino/Alarme.py");
  p.run();

  // clignotement de debug pour dire que le mail est parti
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
}
