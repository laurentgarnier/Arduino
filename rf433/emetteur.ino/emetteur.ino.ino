#include <VirtualWire.h> // On inclue la librairie VirtualWire

char *msg = "0"; // Tableau qui contient notre message

#define LED 8
#define PIN_EMISSION 4
#define ENTREE_CAPTEUR_OUVERTURE 12

int etat;
int etatPrecedent;

void setup() // Fonction setup()
{
 // Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
  vw_setup(2400);        // Initialisation de la librairie VirtualWire à 2400 bauds (maximum de mon module)
  vw_set_tx_pin(PIN_EMISSION);     // On indique que l'on veut utiliser la pin 2 de l'Arduino pour emettre
  pinMode(LED, OUTPUT);
  pinMode(ENTREE_CAPTEUR_OUVERTURE, INPUT_PULLUP);
  etat = HIGH;
  etatPrecedent = etat;
  digitalWrite(LED, etat);
}

void loop() // Fonction loop()
{
  etat = digitalRead(ENTREE_CAPTEUR_OUVERTURE);
  if(etat != etatPrecedent)
  {
      digitalWrite(LED, etat);
      if(etat == LOW)
        msg[0] = char(2);
      else
        msg[0] = char(1);
      
      vw_send((uint8_t *)msg, strlen(msg)); // On envoi le message 
      vw_wait_tx(); // On attend la fin de l'envoi
      etatPrecedent = etat;
   }
}
