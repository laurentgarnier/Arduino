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
from time import sleep

# Horodatage de l'alarme
dateDeLAlarme = datetime.datetime.now()  # on recupere la date actuelle

# creation des donnes du mail
msg = MIMEMultipart()
msg['From'] = "maisoncroziergarnier@gmail.com"
msg['To'] = "l.garnier@tiama.com"
msg['Subject'] = "Ouverture de la porte du jardin"

objetDuMail = "La porte du jardin a ete ouverte"
msg.attach(MIMEText(objetDuMail))

# log sur le serveur de messagerie
mailserver = smtplib.SMTP('smtp.gmail.com', 587)
mailserver.starttls()
mailserver.login('maisoncroziergarnier@gmail.com', 'GMACasLo7774IL')

listeDestinataires = ["l.garnier@tiama.com","ecrozier@wanadoo.fr"]
# envoi a tous les destinataires
for destinataire in listeDestinataires :
	mailserver.sendmail('maisoncroziergarnier@gmail.com', destinataire, msg.as_string())
mailserver.quit()
