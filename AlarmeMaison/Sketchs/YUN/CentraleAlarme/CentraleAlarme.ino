#include <Process.h>
#include <VirtualWire.h> // On inclue la librairie VirtualWire

#define pinReceptionCapteurVeranda 2

uint8_t buf[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message recu (de taille maximum VW_MAX_MESSAGE_LEN)
uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum du buffer

// initialisation du soft
void setup()
{
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    Bridge.begin();
    vw_setup(2400);	// Initialisation de la librairie VirtualWire à 2400 bauds  (maximum de mon module)
    vw_set_rx_pin(pinReceptionCapteurVeranda);
    vw_rx_start();      // Activation de la partie réception de la librairie VirtualWire
}

// boucle principale
void loop()
{
  // Si on a un message dans le buffer
  if (vw_have_message())
  {
    // On récupère ce qu'on a que ce soit complet ou pas
    if (vw_get_message(buf, &buflen))
    {
      int i;
      String msg;
      // On récupère le message
      for (i = 0; i < buflen; i++)
        msg += buf[i];

      // debug
      Serial.print("Reception : " + msg);

      // Code ascii 72 = H correspond à état haut donc porte ouverte
      if( msg.toInt() == 72)
        GererOuvertureVeranda();
    }
  }
}

// Traitement de l'ouverture de la porte de la veranda
void GererOuvertureVeranda()
{
  Process p;
  // Lancement du script python
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sda1/arduino/Alarme.py");
  p.run();

}
