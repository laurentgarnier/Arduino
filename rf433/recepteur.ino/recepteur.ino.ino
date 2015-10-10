#include <VirtualWire.h> // On inclue la librairie VirtualWire

uint8_t buf[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message recu (de taille maximum VW_MAX_MESSAGE_LEN)
uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum du buffer

bool enAttente;
#define LED 11

void setup()
{
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    vw_setup(2400);	// Initialisation de la librairie VirtualWire à 2400 bauds  (maximum de mon module)
    vw_set_rx_pin(2);   // On indique que l'on veut utiliser la broche 2 pour reçevoir
    vw_rx_start();      // Activation de la partie réception de la librairie VirtualWire
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    enAttente = true;
}
void loop()
{
    if(enAttente)
    {
        Serial.println("En attente");
        enAttente = false;
    }
          
    if (vw_have_message()) // Si on a un message dans le buffer
    {
        if (vw_get_message(buf, &buflen)) // Alors on récupère ce message qu'il soit entier ou pas
        {
            int i;
            String msg;
            Serial.print("RX : ");
            for (i = 0; i < buflen; i++) // On affiche tous ce que l'on a
            {
              Serial.print(buf[i]);
              msg += buf[i];
            }
            
            Serial.println(""); // Une fois qu'il est entier, on peut ajouter un retour à la ligne.
            int etat;
            if( msg.toInt() > 1)
              etat = LOW;
            else 
              etat = HIGH;
            digitalWrite(LED, etat);
            enAttente = true;
        }
    }
}
