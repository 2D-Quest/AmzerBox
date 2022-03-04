/*  Description: Programme permettant à la FireBeetle ESP32 d'utiliser l'API OpenWeather pour récupérer des données sur la météo de Brest. 
 *  Les données sont sous un format JSON elles seront ensuite décoder en C. 
 *  Enfin la bibliothèque rgb_lcd.h permettra une communication entre la FireBeetle ESP32 et l'afficheur LCD.
 *  Objectif:  Afficher sur un écran LCD des informations sur la météo de Brest, France.
 *  Matériel: FireBeetle ESP32, Batterie 3.7V 400mAH, fil de câblages.
 *  Auteurs: Harrison Misy, Ange Leyrit.
 *  Date: 17/02/2022.
 *  Version: 1.0.
 */
#include "WiFi.h" // Bibliothèque pour le wifi
#include <HTTPClient.h> // Bibliothèque pour le client http
#include <ArduinoJson.h> // Bibliothèque pour le décodage, du format Json au C.
#include "rgb_lcd.h" // Bibliothèque pour la communication entre l'ESP32 et l'écran LCD.
#include "credentials.h"
rgb_lcd lcd; // Initialise une variable de type rgb_lcd pour pouvoir changer les valeurs à l'écran de l'écran LCD.

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;
Credentials id;
char* URL = id.get_URL();

void information_lcd(){ // Affiche quelles sont les unités des informations.
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature en");
    lcd.setCursor(0,1);
    lcd.print("degre celsius");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Vitesse du vent");
    lcd.setCursor(0,1);
    lcd.print("en km/h");
    delay(2000);
    lcd.clear();
  
}

int connect_wifi() // Vérifie si l'ESP32 est bien connecter au wifi.
{
  if(WiFi.status() == WL_CONNECTED) // si le wifi est connecter retourne la valeur 1
  {
    Serial.println("Connecter au wifi 1");
    return 1;
  }
  else{ // sinon retourne la valeur 0
    Serial.println("Non connecter au wifi 0");
    return 0;
  }
}

void connexion_wifi() // Connecte l'ESP32 au wifi.
{
  
  const char* ssid = id.get_ssid(); 
  const char* password = id.get_password(); 
  lcd.clear();
  lcd.print("Connexion au");
  lcd.setCursor(0,1);
  lcd.print("wifi...");
  Serial.println("\n");
  WiFi.begin(ssid,password);  // Initialisation avec WiFi.begin / ssid et password
  Serial.print("Attente de connexion ...");  // Message d'attente de connexion
  while(WiFi.status() != WL_CONNECTED)  // Test connexion

{

    Serial.print(".");  // Affiche des points .... tant que connexion n'est pas OK

    delay(100);
  }
  

  Serial.println("\n");
  Serial.println("Connexion etablie !");  // Affiche connexion établie
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());  // Affiche l'adresse IP de l'ESP32 avec WiFi.localIP

  
}

void setup() // Setup sera executer seulement une seule fois.
{
    Serial.begin(115200); // Initialise le débit de bit de l'ESP32 115200 bit.
    
    
    lcd.begin(16, 2); // Initialise le nombre de colonnes et de ligne de l'écran LCD (lcd.begin(colonnes, lignes))
    
    lcd.setRGB(colorR, colorG, colorB); // Initialise les couleurs dans l'écran LCD.
    
    lcd.setCursor(0, 0);
    lcd.print("Demat !");
    delay(2000); 
    information_lcd();
    connect_wifi(); // Première connexion au wifi        
}

void loop() // Loop sera tout le temps executer
{
   HTTPClient http; // Initialise une variable de type HTTPClient.
   
   if(connect_wifi() == 0) // si le wifi n'est pas connecter alors la valeur retour de connect_wifi() sera 0
   {
      connexion_wifi(); // Connexion au wifi
   }else // sinon l'ESP32 va envoyer une requete HTTP au serveur OpenWeather via l'API.
   {
      http.begin(URL); //Mettre l'url de votre API.
      int reponseAPI = http.GET(); // Initialise une variable entiere qui contiendra la valeur de la requette Http.
      if(reponseAPI > 0){ // si la variable est superieur à 0 alors la requete a bien était recu.
        Serial.println("REPONSE API OK");
        String input = http.getString(); // Initialise une variable de type string contenant la reponse de l'API OpenWeather
        Serial.println(reponseAPI);
        Serial.println(input);
        StaticJsonDocument<1024> doc; // Initialise un objet de type JsonDocument ayant une taille de 1024 octet.

        DeserializationError error = deserializeJson(doc, input); // Initialise une variable de type DeserializationError contenant le décodage de la reponse recu au format Json en C pouvant etre lue par l'ESP32.
        
        if (error) { // si 1 alors le décodage n'a pas pue etre effectuer.
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }
        // sinon le décodage a était effectuer.
        JsonObject weather_0 = doc["weather"][0]; // Initialise une variable de type JsonObject contenant la valeur à l'indice 0 de l'objet "weather" dans le document json.
        const char* weather_0_description = weather_0["description"]; // Méteo : exemple "Legère pluie".
        JsonObject main = doc["main"]; // Initialise une variable de type JsonObject contenant la valeur de l'objet "main" dans le document json.
        float main_temp = main["temp"]; // Temperature
        float wind_speed = doc["wind"]["speed"]; // Vitesse du vent    
        lcd.clear();
        lcd.setCursor(0, 0); // Met le curseur sur la colonne 0 à la ligne 0
        lcd.print("T:");
        lcd.print(main_temp); // Affiche la temperature
        lcd.setCursor(0, 1); // Met le curseur sur la colonne 0 à la ligne 1
        if(doc["weather"][0]["description"] == "légère pluie"){
          lcd.print("legere pluie"); // Affiche legere pluie car on ne peux pas écrire les accents sur l'écran LCD. Exemple: "Legère pluie".
        }else{
          lcd.print(weather_0_description);// Affiche la météo.
        }
         
        lcd.setCursor(8, 0); // Met le curseur sur la colonne 8 à la ligne 0
        lcd.print("V:");
        lcd.print(wind_speed*1.85); // Affiche la vitesse du vent en noeuds.
        delay(60000); // Actualise les informations toutes les 1 minutes.
        
      }else{ // Sinon alors la requete http n'a pas pue etre envoyé au serveur.
        Serial.println("Erreur de requete HTTP");
        delay(1000);
      }
   }
   
}
