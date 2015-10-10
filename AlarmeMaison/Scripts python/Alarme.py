# -*- coding: utf-8 -*-

import httplib
import os
import datetime
import time
import smtplib

from os.path import basename
from email.MIMEMultipart import MIMEMultipart
from email.mime.application import MIMEApplication
from email.MIMEText import MIMEText
from email.MIMEBase import MIMEBase
from email.MIMEImage import MIMEImage
from email import encoders

# Horodatage de l'alarme
dateDeLAlarme = datetime.datetime.now()  # on recupere la date actuelle
timestampDeLAlarme = time.mktime(dateDeLAlarme.timetuple())

adresseCamera = "192.168.1.81"

# Creation du repertoire de stockage des images de l'alarme
repertoire = os.path.join("/mnt/sda1/arduino/Captures", str(timestampDeLAlarme))
os.mkdir(repertoire, 0777)

# Lancement des captures d'image depuis la camera
httpServ = httplib.HTTPConnection(adresseCamera, "81")
httpServ.connect()
# Lancement des acquisitions
for indexImage in range(10) :
    httpServ.request('GET', "/snapshot.cgi?user=admin&pwd=caslo7774")
    # attente de la reponse
    response = httpServ.getresponse()
    # Recuperation de l'image
    donnees = response.read()
    # Sauvegarde de l'image
    fichier = open(repertoire + "/Veranda" + "_" + str(timestampDeLAlarme) + "_" + str(indexImage) + ".png", 'w')
    fichier.write(donnees)
    fichier.close()

httpServ.close()

# envoi du mail

# Liste les images du répertoire
fichiers = os.listdir(repertoire)
# creation des donnes du mail
msg = MIMEMultipart()
msg['From'] = "lologar69@gmail.com"
msg['To'] = "l.garnier@msc-sgcc.com"
msg['Subject'] = "Ouverture de la porte de la veranda"

objetDuMail = "La veranda a ete ouverte"
msg.attach(MIMEText(objetDuMail))

# on ajoute les fichiers en pieces jointes
for f in fichiers or []:
        with open(os.path.join(repertoire, f), "rb") as fil:
            msg.attach(MIMEImage(
                fil.read(),
                Content_Disposition='attachment; filename="%s"' % basename(f)
            ))

# log sur le serveur de messagerie
mailserver = smtplib.SMTP('smtp.gmail.com', 587)
mailserver.starttls()
mailserver.login('lologar69@gmail.com', 'laur*nt2')

listeDestinataires = ["l.garnier@msc-sgcc.com","estelle.crozier@wanadoo.fr"]
# envoi a tous les destinataires
for destinataire in listeDestinataires :
	mailserver.sendmail('lologar69@gmail.com', destinataire, msg.as_string())
mailserver.quit()
