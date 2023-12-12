Setup
=====

Dans le fichier `idf_component.yml` vous devrez ajouter les dépendances suivantes :
```
  espressif/asio: "*"
  bblanchon/arduinojson: "^6.21.3"
```

Ajout du telemetre ultrason
===========================

En vous inspirant de l'exemple `esp-idf-cxx/examples/hc_sr04_cxx` vous devrez ajouter le telemetre à ultrason dans votre projet.
Vous devrez créer un thread dédié à ce capteur effectuant une mesure régulière et dans un premier temps la logant sur le port série (pour la voir sur la sortie monitor).
Vous devrez modifier la classe HcSr04 :

- Avoir un fichier d'entête et un fichier source
- Pour ne plus avoir de flottant dans la fonction `receive` et faire la convertion en utilisant que des entiers pour diminuer la charge CPU et du coup retourner une valeur en mm
- Ajouter en paramètre du constructeur le groupIp pour le membre _capTimer(mcpwmGroupId)
- Modifier la queue _rspQueue pour ne considérer que la valeur la plus récente

Ajout du Wifi
=============

Pour ne pas à avoir à mettre le SSID et le mot de passe dans le repot, vous devrez les ajouter en passant par le menu config (icone roue dentée en bas) :
- ajouter le fichier `Kconfig.projbuild`dans le dossier `main` avec le contenu suivant :
```
    menu "Robot ESP IDF Configuration"

        config ESP_WIFI_SSID
            string "WiFi SSID"
            default "myssid"
            help
                SSID (network name) for the example to connect to.

        config ESP_WIFI_PASSWORD
            string "WiFi Password"
            default "mypassword"
            help
                WiFi password (WPA or WPA2) for the example to use.

    endmenu
```
- Lancer le menu config pour saisir les valeurs, de ces 2 nouveaux paramètres (il faudra peut être faire un reset mais n'oubliez pas de remettre les valeurs du premier TP), de plus dans le champ `Partition Table`il faudra changer la valeur à `Single factory app(large), no OTA` sinon vous aurez une erreur de taille de binaire trop grande pour la partition principale.
- En vous inspirant de l'exemple `esp-idf-cxx/examples/wifi_cxx` vous devrez activer le wifi pour pouvoir étabir une connexion TCP/IP entre l'ESP et votre ordinateur.

Ajout du server Json RCP TCP
============================

En ajoutant les 2 fichiers sources de l'archive `robot-embedded-TP3-jsonrpctcpserver.tar.gz`dans le dossier main (et en ajoutant le fichier source dans le `CMakeLists.txt`, vous devrer instancier le serveur json RCP TCP puis lancer la fonction listen() qui fera office de boucle `while(true)` dans le main. Voici un exemple de code :
```
    // Create an instance of Json RCP server on TCP port 6543
    JsonRpcTcpServer jsonRpcTcpServer(6543);

    // When a new client connect send it a jsonRCP notification
    // with name "setIsReady" and empty param
    jsonRpcTcpServer.bindOnConnectSendNotification("setIsReady",
            [](){return ArduinoJson::DynamicJsonDocument(10);});

    // Wait and serve new client
    jsonRpcTcpServer.listen();
```
Vous devez connecté l'ESP32 et l'orinateur sur le même réseau Wifi, il y a 3 méthodes :

- L'ESP32 en mode AP (accès point) et le PC se connecte en Wifi à l'AP de l'ESP32
- L'ordinateur en mode accès point et l'ESP32 en mode station pour ce connecté dessus
- Le téléphone en mode partage de connection par le Wifi, et l'ESP32 et l'ordinateur se connecte dessus

Depuis l'ordinateur, si tout est bien configuré, vous devrez pouvoir recevoir le message `setIsReady` à l'aide de la commande netcat. Voici un exemple d'utilisation :
```
    netcat 192.168.1.2 6543
    {"jsonrpc":"2.0","method":"setIsReady","params":null}
```

Ensuite, dans le thread du telemetre ultrason, vous devrez remplacer le log sur le port série par l'envoi de la notification `ultrasoundDistanceDetected` vers le client en uilisant la fonction `sendNotification()` du server Json RCP avec les paramètres :

- `index` : valeur entiere du numéro du capteur en commençant par 0 (donc ici toujours 0)
- `value` : valeur entiere mesurée par le capteur (en mm)
- `changedCount` : valeur entiere auto incrementée, pour que le client vérifie s'il n'y a pas eu de message raté

Ajout du controle des moteurs
=============================

Vous devrez instancier les 2 moteurs comme pour le 2eme TP et à l'aide de la fonction `bindNotification` vous devrez piloter le ou les moteurs à la réception d'un message :

- `setMotorSpeed` contenant les paramètres :
    - `motorIndex` : avec comme valeur `LEFT` ou `RIGHT`
    - `value`: avec comme valeur un float entre -1.0 et 1.0
- `setMotorsSpeed` contenant les paramètres :
    - `leftValue` : avec comme valeur un float entre -1.0 et 1.0
    - `rightValue`: avec comme valeur un float entre -1.0 et 1.0

Attention vous ne pouvez pas modifier la vitesse du moteur directement dans la lamda du `bindNotification` car il y a des problèmes de contrainte de temps réel (le module réseau de l'ESP lève une erreur si on ne rend pas la main rapidement). Vous devrez donc utiliser la classe `idf::Queue` pour vous poster un message et le traiter dans un thread dédié au controle des moteurs.

Test final
==========

Sur le projet robot-command (utilisé par votre binôme) il y a une branche pour contrôler le robot au clavier avec affichage de l'ultrason. Voici les commandes pour le récupérer et vous avez dans le README.md les instructions pour le compiler.
```
   git clone git@gitlab.cri.epita.fr:jeremie.graulle/ssie-s9-robot-command.git
   git checkout robotKeyboard
```
Vous devrez le lancer en ajoutant en 1er argument l'IP de l'ESP32 et en 2ème argument le port TCP de votre serveur Json RCP.
Il y aura une petite démo à faire avec le contrôle clavier et affichage du télémètre.
