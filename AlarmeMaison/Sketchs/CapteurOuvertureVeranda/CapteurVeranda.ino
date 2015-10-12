#include <VirtualWire.h> // On inclue la librairie VirtualWire

char *msg = "0"; // Tableau qui contient notre message

#define LED 8
#define PIN_EMISSION 4
#define ENTREE_CAPTEUR_OUVERTURE 12

int etat;
int etatPrecedent;

void setup()
{
  vw_setup(2400);        // Initialisation de la librairie VirtualWire à 2400 bauds (maximum de mon module)
  vw_set_tx_pin(PIN_EMISSION);     // On indique que l'on veut utiliser la pin 2 de l'Arduino pour emettre
  pinMode(LED, OUTPUT);
  pinMode(ENTREE_CAPTEUR_OUVERTURE, INPUT_PULLUP);
  etat = HIGH;
  etatPrecedent = etat;
  digitalWrite(LED, etat);
}

void loop()
{
  etat = digitalRead(ENTREE_CAPTEUR_OUVERTURE);
  if(etat != etatPrecedent)
  {
      digitalWrite(LED, etat);
      // Etat bas pour capteur fermé, donc porte fermée et H quand porte ouverte
      if(etat == LOW)
        msg[0] = 'L';
      else
        msg[0] = 'H';

      vw_send((uint8_t *)msg, strlen(msg)); // On envoit le message
      vw_wait_tx(); // On attend la fin de l'envoi
      etatPrecedent = etat;
   }
}
