  const int redCarPin = 6;
  const int yellowCarPin = 7;
  const int greenCarPin = 8;
  const int greenPietonPin = 9;
  const int redPietonPin=10;
  const int buttonPin=3;
  
  int stateButton=0; // Soit pressé ou pas pressé
  boolean buttonIsPressable=true; // va permettre le blocage
  boolean seq_terminee=false;
  
  int state=-1;

// Gestion du temps en ms
  unsigned long const waitBetween=20000;
  unsigned long const waitYellowFix=3000;
  unsigned long const waitRedFix=3000;
  unsigned long const waitRedToGreenPieton=3000;
  unsigned long const waitGreenPietonFix=10000;
  unsigned long const waitCarPieton=4000;
  unsigned long const waitGreenToYellow = 5000;
  unsigned long const clignGreenPieton=3000;
  
  unsigned long  previousTime=0;
  unsigned long  previousTimeCli=0;
void setup() {     
  Serial.begin(9600);
// Feu voiture 
  pinMode(redCarPin,OUTPUT);
  pinMode(yellowCarPin,OUTPUT);
  pinMode(greenCarPin,OUTPUT);

// Pieton
  pinMode(redPietonPin,OUTPUT);
  pinMode(greenPietonPin,OUTPUT);

//Bouton
  pinMode (buttonPin,INPUT);
}

void loop() {
  // Point de départ
  digitalWrite(greenCarPin,LOW);
  digitalWrite(redCarPin,LOW);
  digitalWrite(yellowCarPin,HIGH);
  digitalWrite(redPietonPin,HIGH);
  digitalWrite(greenPietonPin,LOW);


 stateButton = digitalRead(buttonPin);

// Va stocker le temps en ms depuis le lancement de l'arduino
  unsigned long  currentTime=millis();


  // Etat en attente => le bouton orange clignote toutes les 0,5 secondes.
  if(currentTime - previousTimeCli >= 500){
     previousTimeCli = currentTime; // Pour répéter l'opération toutes les 0,5 sec
     if (digitalRead(yellowCarPin) == LOW){
        digitalWrite(yellowCarPin,HIGH);
     }else{
        digitalWrite(yellowCarPin,LOW);
     }
    if(stateButton==HIGH && buttonIsPressable==true ){
      buttonIsPressable=false; // 
      state==0; // Permet de démarrer la séquence
    }else if(stateButton == HIGH && buttonIsPressable==false){// <= BLOQUER L'APPUI. 
      // Ne rien faire
      }
  }

 if(state==0){ // 3 sec d'orange fixe, éteindre orange et allumer rouge
      currentTime=0;
      digitalWrite(yellowCarPin,HIGH);
      if(currentTime - previousTime >= waitYellowFix){
        digitalWrite(yellowCarPin,LOW);
        digitalWrite(redCarPin,HIGH);
        state==1;
      }
 }else if(state==1){ // Allumer le feu 10 secondes pour piéton
    currentTime=0;
    digitalWrite(redPietonPin,LOW);
    digitalWrite(greenPietonPin,HIGH);
    // attendre 10 sec en vert
    if(currentTime-previousTime >= waitGreenPietonFix){
      // clignoter toutes les 0,5 secs
      if(currentTime-previousTime >= 500){
        if (digitalRead(greenPietonPin) == LOW){
          digitalWrite(greenPietonPin,HIGH);
        }else{
          digitalWrite(greenPietonPin,LOW);
        }
        previousTime = currentTime; // Pour répéter l'opération toutes les 0,5 sec
      }
      state==2;
    }
 }else if(state==2){ // Eteindre le vert, allumer le rouge du piéton mais allumer la verte au bout de 4 secs
    currentTime=0;
    digitalWrite(redPietonPin,HIGH);
    digitalWrite(greenPietonPin,LOW);

    // 4 sec d'attente
    if(currentTime - previousTime >= waitCarPieton){
      digitalWrite(redCarPin,LOW);
      digitalWrite(greenCarPin,HIGH);
      seq_terminee=true;
    }
    // Mettre une attente de 20 sec à la fin de la séquence pour débloquer l'utilisation du bouton.
    if(currentTime-previousTimeCli >= waitGreenToYellow && seq_terminee==true){
      if(currentTime-previousTimeCli >= waitBetween){
          buttonIsPressable==false;
      }
    }
 }
 }
