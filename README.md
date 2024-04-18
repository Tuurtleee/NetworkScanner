# NetworkScanner
Projet RSA réalisé par Erwan VAILLANT et Thomas LACROIX

## Installation
Pour installer le projet, il suffit de cloner le dépôt git et de faire un make:
```bash
make mainClient && make mainServer
``` 

## Utilisation
Pour lancer le serveur, il suffit de lancer la commande suivante:
```bash
sudo ./mainServer
```
ATTENTION: Il est nécessaire de lancer le serveur en tant que super utilisateur pour pouvoir envoyer des paquets ICMP. (pour le scan de réseau)

Pour lancer le client, il suffit de lancer la commande suivante:
```bash
./mainClient
```


Dans le client, un shell est disponible pour envoyer des commandes au serveur. Les commandes disponibles sont les suivantes:
- `scan` : Permet de scanner le réseau local
    Paramètres:
        - Pour un scan horizontal: `scan 0 -targetH XXXX.XXXX.XXXX.XXXX -mask XXXX.XXXX.XXXX.XXXX`
        - Pour un scan vertical: `scan 1 -targetV XXXX.XXXX.XXXX.XXXX` par défaut l'adresse IP est celle du serveur
- `help` : Affiche l'aide
- `exit` : Quitte le client et libère les ports

