/*******************************************************************************/
/* Definition des entrees / sorties */
#define CAPTEUR_MOUVEMENT 2
#define SORTIE	4

void setup()
{
	initialiserEntreesSorties();
}

void loop() 
{
	if(mouvementDetecte())
		activerSortie();
	else
		desactiverSortie();
}
void initialiserEntreesSorties()
{
	pinMode(CAPTEUR_MOUVEMENT, INPUT);
	pinMode(SORTIE,OUTPUT);
}
void activerSortie()
{
	digitalWrite(SORTIE,HIGH);
}
void desactiverSortie()
{
	digitalWrite(SORTIE,LOW);
}
/***************************************************************/
/*Function: detection d'un mouvement */
/*Return:-boolean, true si un mouvement est detecte.*/
boolean mouvementDetecte()
{
	int valeurDuCapteur = digitalRead(CAPTEUR_MOUVEMENT);
	
        return (valeurDuCapteur == HIGH);
}
