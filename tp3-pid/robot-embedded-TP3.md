Setup
=====

Ajout du telemetre ultrason
===========================

En vous inspirant de l'exemple `esp-idf-cxx/examples/hc_sr04_cxx` vous devrez ajouter le telemetre à ultrason dans votre projet.
Vous devrez créer un thread dédié à ce capteur effectuant une mesure régulière et dans un premier temps la logant sur le port série (pour la voir sur la sortie monitor).
Vous devrez modifier la classe HcSr04 :

- Avoir un fichier d'entête et un fichier source
- Pour ne plus avoir de flottant dans la fonction `receive` et faire la convertion en utilisant que des entiers pour diminuer la charge CPU et du coup retourner une valeur en mm
- Ajouter en paramètre du constructeur le groupIp pour le membre _capTimer(mcpwmGroupId)
- Modifier la queue _rspQueue pour ne considérer que la valeur la plus récente

Ensuite, dans le thread du telemetre ultrason, vous devrez remplacer le log sur le port série par l'envoi de la notification `ultrasoundDistanceDetected` vers le client en uilisant la fonction `sendNotification()` du server Json RCP avec les paramètres :

- `index` : valeur entiere du numéro du capteur en commençant par 0 (donc ici toujours 0)
- `value` : valeur entiere mesurée par le capteur (en mm)
- `changedCount` : valeur entiere auto incrementée, pour que le client vérifie s'il n'y a pas eu de message raté

Ajout des interrupteurs
=======================

Ajout du télémètre IR
=====================

Test final
==========

Sur le projet robot-command (utilisé par votre binôme) il y a une branche pour contrôler le robot au clavier avec affichage de l'ultrason. Voici les commandes pour le récupérer et vous avez dans le README.md les instructions pour le compiler.
```
   git clone git@gitlab.cri.epita.fr:jeremie.graulle/ssie-s9-robot-command.git
   git checkout robotKeyboard
```
Vous devrez le lancer en ajoutant en 1er argument l'IP de l'ESP32 et en 2ème argument le port TCP de votre serveur Json RCP.
Il y aura une petite démo à faire avec le contrôle clavier et affichage du télémètre.
