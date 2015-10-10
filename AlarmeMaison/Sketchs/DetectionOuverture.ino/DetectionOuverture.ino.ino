#include <Process.h>

// pin 2 de l'arduino
const int entreeCapteurOuverture = 2;

int etat = LOW;
int dernierEtat = LOW;

void setup() {
  Bridge.begin();
  pinMode(entreeCapteurOuverture, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  digitalWrite(13, etat);
}

void loop() {
    etat = digitalRead(entreeCapteurOuverture);
    digitalWrite(13, etat);
    
    if(etat == LOW && etat != dernierEtat) {
        
        Process p;
        // Lancement du script python
        //String cmd = "python /mnt/sda1/arduino/Snapshot2.py";
        p.begin("/usr/bin/python");
        p.addParameter("/mnt/sda1/arduino/Alarme.py");
        p.run();
        digitalWrite(12, HIGH);
    }
    else
        digitalWrite(12, LOW);
 
    dernierEtat = etat;
    
}

