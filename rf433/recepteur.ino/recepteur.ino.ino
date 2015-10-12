#include <VirtualWire.h> // On inclue la librairie VirtualWire

uint8_t buf[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message recu (de taille maximum VW_MAX_MESSAGE_LEN)
uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum du buffer

bool enAttente;
#define LED 11
#define PIN_RECEPTION 2

void setup()
{
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    vw_setup(2400);	// Initialisation de la librairie VirtualWire à 2400 bauds  (maximum de mon module)
    vw_set_rx_pin(PIN_RECEPTION);
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

    // Si on a un message dans le buffer
    if (vw_have_message())
    {
        // On récupère ce qu'on a complet ou pas
        if (vw_get_message(buf, &buflen))
        {
            int i;
            String msg;
            // On affiche le message
            for (i = 0; i < buflen; i++)              
              msg += buf[i];
          
            Serial.print("Reception : " + msg);

            // Une fois qu'il est entier, on peut ajouter un retour à la ligne.
            Serial.println("");

            // on traite le message
            int etat;
            if( msg.toInt() == 76)
              etat = LOW;
            else
              etat = HIGH;
            digitalWrite(LED, etat);
            enAttente = true;
        }
    }
}
