/*  Description: Dans ce fichier il faut rentrer le ssid, le mot de passe wifi et l'URL de votre API dans les fonctions (respectivement) get_ssid, get_password et get_URL, pour pouvoir les
 *  réutiliser dans le fichier principal.
 *  Objectif:  Sauvegarder les données critiques dans une bibliothèque.
 *  Auteurs: Harrison Misy, Ange Leyrit.
 *  Date: 18/02/2022.
 *  Version: 1.0
 */


#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__

// Wifi parameters
class Credentials{ // Classe pour les données critiques, mot de passe, identifiant, clé de connexion, etc..
    private:
        char* ssid = "nomreseauwifi"; // nom réseau wifi.
        char* password = "motdepasse"; // Mot de passe du réseau wifi.
        char* URL = "URL_API_KEY";
    public:
        char* get_ssid(){ // Fonction permettant d'accéder à la variable privée ssid, elle peut être appeler dans le fichier source "apiopenweather.ino"
            return ssid; 
        }
        char* get_password(){ // Fonction permettant d'accéder à la variable privée password, elle peut être appeler dans le fichier source "apiopenweather.ino"
            return password;
        }
        char* get_URL(){
            return URL;
        }
};



#endif