Robot - Simu et commande : TP3
==============================

Installation
------------

- Avoir le projet `robot-simu` dans le même état que pour le TP2.
- Sur le projet robot-command, créer une nouvelle branche `tp3` depuis la branche `main` ou depuis
votre branche `tp2`.

Sujet
-----

Dans le projet `robot-command`, vous devez remplacer la boucle principale pour réaliser un
asservissement de la position du robot par rapport à une distance mesurée par le capteur à ultrason
à l'aide d'un PID.

Vous trouverez une explication du PID sur la page [Wikipédia](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller).

Vocabulaire utilisé :
- `current` (courant) : valeur courant que l'on a mesurée, ici la distance au premier
  obstacle mesuré par le capteur ultrason.
- `target` (cible) : valeur que l'on cherche a atteindre, ici également la distance au premier
  obstacle donné par le programme.
- `command` (commande) : valeur permettant d'agir sur le système, ici la puissance appliquée aux
  moteurs.
- `error` (erreur) : la différence entre la valeur courant et la valeur cible.
- `dt` (delta time) : temps écoulé depuis le dernier calcul d'asservissement.

Vous devrez créer une classe dédiée pour le PID, en suivant l'exemple d'en-tête suivant :
```
    class PidController
    {
    public:
        PidController(float commandMin, float commandMax, float kp, float ki, float kd);
        ~PidController();

        inline float getKp() const {return _kp;}
        inline float getKi() const {return _ki;}
        inline float getKd() const {return _kd;}
        inline float getTarget() const {return _target;}

        void setTarget(float value);
        float update(float dt, float current);

    private:
        const float _commandMin;
        const float _commandMax;
        const float _kp;
        const float _ki;
        const float _kd;
        float _target;
        // ...
    };
```

Vous devrez régler les 3 coefficients du PID les uns après les autres en commençant par le P, puis
le I et enfin le D. Pour chaque réglage, vous devrez générer une courbe (en utilisant par exemple
un tableur ou gnuplot) montrant au moins 5 valeurs pertinentes de coefficient que vous avez
testées avec au moins un changement de valeur cible. Je vais vous demander de trouver le bon
compromis entre rapidité (vitesse pour atteindre une valeur proche de la cible) et exactitude
(erreur finale).

Vous devez :

- Avoir un fichier d'entête et un fichier source
- Ajouter des commentaires décrivant les étapes principales
- Ajouter des commentaires au format Doxygen à la classe PID
- Ajouter au moins 1 changement de valeur cible après x secondes
- Les fonctions de la classe PID doit être robuste (protéger contre les paramètres invalides)
- Dans la fonction update vous devrez ajouter en commentaire les principales étapes
- Utiliser des vrais constantes pour les constantes littérales
- Ajouter dans le git les courbes au format Excel, OpenOffice, SVG, PNG ou PDF (peut-être un format
Excel ou LibreOffice en plus d'un format SVG, PNG ou PDF serait l'idéal dans le cas où je n'arrive
pas bien à ouvrir le fichier Excel ou LibreOffice). Pour chaque courbe :
    - Vous devez avoir une échelle de valeur sur chaque axe avec un titre et une unité
    - Afficher la courbe de la valeur cible
    - Vous devez avoir 2 coefficients fixes dont les valeurs sont dans le titre et un seul
      coefficient qui varie dont les valeurs sont dans la légende
        - Courbe KP : KI et KD sont fixés à 0
        - Courbe KI : KP est fixé à la valeur qui vous semble la meilleure parmi celles testées dans
          le graphe précédent
        - Courbe KD : KP est fixé à la même valeur que dans le précédent graphe et KI est fixé à
          la valeur qui vous semble la meilleure parmi celles testées dans le graphe précédent

![Exemple de réglage de la valeur P](tp3-pid/robot-simu-TP3-PID-KP.svg)

![Exemple de réglage de la valeur I](tp3-pid/robot-simu-TP3-PID-KI.svg)

![Exemple de réglage de la valeur D](tp3-pid/robot-simu-TP3-PID-KD.svg)

