#include <LiquidCrystal.h> // Inclusion de la librairie pour afficheur LCD
#include <Wire.h>
#include <RTClib.h>
#include <Keypad.h> // inclusion de la librairie pour clavier matriciel
//#include <DS1307.h>
//#include <GSM.h>

const int RS=11; 
const int E=10; 
const int D4=9; 
const int D5=8; 
const int D6=7; 
const int D7=6; 


 const byte ROWS=4;
 const byte COLS=4;
 char touches[ROWS][COLS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
 };
  byte rowPins [ROWS]={5,4,3,2};
 byte colPins [COLS]={14,15,16,17};
 Keypad clavier= Keypad ( makeKeymap(touches), rowPins, colPins, ROWS, COLS);
 

// --- Déclaration des constantes des broches analogiques ---


// --- Déclaration des variables globales ---
char phone_no[]="+21650082707";


//--- Définition des touches



char touche; // variable de stockage valeur touche appuyée

unsigned int code,codeReceived; // variable pour code saisi
unsigned int secret=1111; // variable pour code saisi
unsigned int tempo=10; // variable pour code saisi
unsigned int temp1;
int intrus1;
int cnt1;
int intrus2;
int cnt2;
int capteur1=30; // capteur pour zone1
int contact1=31; // conctact pour zone1
int capteur2=34; // capteur pour zone2
int contact2=35; // contact pour zone2
int buzzer=12;
int tamper=20;
int k;

char senderNumber[20];



// --- Déclaration des objets utiles pour les fonctionnalités utilisées ---

//------------ Création d'un objet afficheur LCD alphanumérique ----
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);// initialisation LCD en mode 4 bits

// création d'un objet keypad = initialisation clavier
//Keypad clavier = Keypad( makeKeymap(touches), BrochesLignes, BrochesColonnes, LIGNES, COLONNES );
// les broches de lignes sont automatiquement configurées en ENTREE avec pullup interne activé
// les broches de colonnes sont automatiquement configurées en SORTIE

RTC_DS1307 RTC;

// ////////////////////////// 2. FONCTION SETUP = Code d'initialisation //////////////////////////
// La fonction setup() est exécutée en premier et 1 seule fois, au démarrage du programme

void setup()   { // debut de la fonction setup()

// --- ici instructions à exécuter 1 seule fois au démarrage du programme ---

// ------- Initialisation fonctionnalités utilisées -------  
  Serial.begin(115200);
  delay(300);
// Initialisation de l'afficheur LCD
Wire.begin();
RTC.begin();
lcd.begin(20,4); // Initialise le LCD avec 20 colonnes x 4 lignes
delay(10); // pause rapide pour laisser temps initialisation
lcd.clear(); // efface LCD
delay(10);
lcd.blink(); // active clignotement
pinMode(capteur1, INPUT);
pinMode(contact1, INPUT);
pinMode(capteur2, INPUT);
pinMode(contact2, INPUT);
pinMode(tamper, INPUT);
pinMode(buzzer, OUTPUT);
DateTime dt = DateTime(__DATE__, __TIME__);
  RTC.adjust(dt);}


void loop(){ // debut de la fonction loop()

  digitalWrite(buzzer,LOW);
  lcd.clear();
  horloge();
  lcd.setCursor(0, 1); 
  lcd.blink();
  lcd.print("Saisir touche:"); // affiche variable
  code=SAISIE_uINT(); // appel de la fonction pour saisie valeur - # stoppe saisie
  

codeReceived=ReceptionSmS();
if(code==0){
  lcd.clear();
  horloge();
  lcd.setCursor(0, 1); 
  lcd.print("Saisir touche:"); 
  lcd.blink();
 
}
  
if(code==1 ||codeReceived==1){
temp1=tempo;
    while (temp1!=0)
      {
      lcd.clear();
      horloge();
      lcd.setCursor(0, 1); 
      lcd.print("Zone 1 active:"); // affiche variable
      lcd.print(temp1);
      lcd.noBlink();
      delay(1000);
      temp1=temp1-1;
      }
    do
      {
      lcd.clear();
      horloge();
      lcd.setCursor(0, 1); 
      lcd.print("Zone 1 active");
      delay(100);
      intrus1=digitalRead(capteur1);
      cnt1=digitalRead(contact1);
      }
    while ((intrus1==0)&&(cnt1==0));
    delay(1000);
    lcd.clear();
    lcd.print("Desactiver zone");
    lcd.setCursor(0, 1);
    lcd.blink();
    code=SAISIE_uINT();
    codeReceived=ReceptionSmS();

    if (code!=secret || codeReceived !=secret)
       {
        digitalWrite(buzzer,HIGH);
        SMS();
        lcd.clear();
        horloge();
        lcd.setCursor(0, 1);
        lcd.print("alarme zone1 ");
        
       
ici1:    code=SAISIE_uINT();
        if (code==secret || codeReceived == secret)
        {
        break;
        }
        else {//goto ici1;
        delay(12000);
        digitalWrite(buzzer,LOW);}
       }
      break;
  
if(code==2 ||codeReceived==2){
      lcd.clear();
      horloge();
      lcd.setCursor(0, 1); 
      lcd.print("Zone 2 active");
      delay(1000);
      lcd.noBlink();
      intrus2=digitalRead(capteur2);
      cnt2=digitalRead(contact2);
      }
      while ((intrus2==0)&&(cnt2==0));
    lcd.clear();
    digitalWrite(buzzer,HIGH);
    SMS();
    lcd.clear();
    horloge();
    lcd.setCursor(0, 1);
    lcd.print("alarme zone2 ");

ici2:    code=SAISIE_uINT();
codeReceived=ReceptionSmS();

        if (code==secret || codeReceived==secret)
        {
        break;
        
        }
        else {//goto ici2;
              delay(12000);
              digitalWrite(buzzer,LOW);}
        
  break;
  
if(code==3 ||codeReceived==3){

   temp1=tempo;
    while (temp1!=0)
      {
      lcd.clear();
      horloge();
      lcd.setCursor(0, 1); 
      lcd.print("Zones actives:"); // affiche variable
      lcd.print(temp1);
      lcd.noBlink();
      delay(1000);
      temp1=temp1-1;
      }
    do
      {
      lcd.clear();
      horloge();
      lcd.setCursor(0, 1); 
      lcd.print("Zones actives");
      delay(100);
      intrus1=digitalRead(capteur1);
      cnt1=digitalRead(contact1);
      intrus2=digitalRead(capteur2);
      cnt2=digitalRead(contact2);
      }
    while ((intrus1==0)&&(cnt1==0)&&(intrus2==0)&&(cnt2==0));
    delay(1000);
        if ((intrus1==1)||(cnt1==1))
        {
        lcd.clear();
        lcd.print("Desactiver zones");
        lcd.setCursor(0, 1);
        lcd.blink();
        code=SAISIE_uINT();
        codeReceived=ReceptionSmS();

          if (code!=secret || codeReceived != secret)
          {
          digitalWrite(buzzer,HIGH);
          SMS();
          lcd.clear();
          horloge();
          lcd.setCursor(0, 1);
          lcd.print("alarme zone1 ");
         
ici3:    code=SAISIE_uINT();
codeReceived=ReceptionSmS();

             if (code==secret ||codeReceived == secret)
             {
              break;
             }
             else {//goto ici3;
              delay(12000);
          digitalWrite(buzzer,LOW);}
           }
         }
         else if ((intrus2==1)||(cnt2==1))
         {
         lcd.clear();
         digitalWrite(buzzer,HIGH);
         SMS();
         lcd.clear();
         horloge();
         lcd.setCursor(0, 1);
         lcd.print("alarme zone2 ");
         
ici4:    code=SAISIE_uINT();

codeReceived=ReceptionSmS();

          if (code==secret )
          {
          break;
          }
          else {//goto ici4;
          delay(12000);
         digitalWrite(buzzer,LOW);}
          }
      break;

   if (code==7){
  lcd.clear();
  lcd.print("armement"); // affiche variable
  digitalWrite(buzzer,HIGH);
  delay(12000);
  digitalWrite(buzzer,LOW);
  break;
   }
 if (code==8){
  lcd.clear();
  lcd.print("Temporisation"); // affiche variable
  lcd.setCursor(0,1);
  lcd.print("(1..99 s):");
  lcd.blink();
  code=SAISIE_uINT();
  tempo=code;
  break;    }
  
  
  
if (code==9){ 
  lcd.clear();
}
  horloge();
  lcd.setCursor(0, 1); 
  lcd.print("Code secret:"); // affiche variable
  lcd.blink();
  code=SAISIE_uINT();
  if (code==secret)
    {
     lcd.clear();
     lcd.print("Nouveau code:"); // affiche variable
     lcd.setCursor(0, 1); 
     lcd.blink();
     code=SAISIE_uINT();
     secret=code;
    }
  else
  {
  lcd.clear(); 
  lcd.print("Code errone:"); // affiche variable
  lcd.noBlink();
  delay(1000);}  
 
  lcd.clear();
  horloge();
  lcd.setCursor(0,1);
  lcd.print("Touche inactive"); // affiche variable
  lcd.noBlink();
  delay(1500); // pause entre 2 saisies
  }
  


}
}// fin de la fonction loop() - le programme recommence au début de la fonction loop sans fin
unsigned int SAISIE_uINT (){ // fonction qui renvoie une valeur unsignedint (16bits)

//---- variables locales utilisées dans la fonction
unsigned char saisie_int[8]={0,0,0,0,0,0,0,0}; // crée un tableau de variables char pour saisir valeur
unsigned char compt_touche_int; // variable de comptage du nombre de touches saisies
unsigned long nombre_saisi_int; // variable de calcul du nombre saisi
unsigned char touche_int; // variable locale pour valeur touche appuyée de 0 à 15
unsigned int temp;
unsigned int no_key;
unsigned int debut;
//---- initialisation des variables utilisées ----
compt_touche_int=0; // aucune touche saisie au démarrage
nombre_saisi_int=0; // initialise la variable

//------ gestion appui touche sur clavier
 debut=millis();
 while (compt_touche_int<5){ // tant que pas appuis maxi = 5 pour int (valeur maxi=65535)

       touche_int= clavier.getKey(); // lecture de la touche appuyée
        if (touche_int== NO_KEY)  // si clavier inutilisé pendant 10 secondes, on sort de la fonction
        {
          no_key=millis();
          temp=no_key-debut;
          if (temp>10000) break;
        }
       if (touche_int!= NO_KEY){ // si une touche a été frappée -- gestion de la touche appuyée


           if (touche_int=='#') break; // sort du while si appui sur #

           for (byte i=0;i<ROWS;i++) {
             for (byte j=0; j<COLS;j++){

               if (touche_int==touches[i][j]) touche_int=touches[i][j]-48; // extrait valeur numérique à partir ASCII
             }
           }
           saisie_int[compt_touche_int]=touche_int; //enregistre le caractère dans tableau code_saisi
           lcd.print(char(saisie_int[compt_touche_int]+48)); // affiche valeur à emplacement actuel sur LCD

           compt_touche_int=compt_touche_int+1; // on incrémente la variable compt qui mémorise le nombre de caractères saisis

           //delay ()

           touche_int=255; // remet à 255 la variable touche... pour éviter prise en compte même appui à plusieurs reprises

         } //---- fin gestion appui clavier

}// fin du While = fin de la lecture du clavier

//--------- calcul valeur saisie en fonction du nombre de chiffres saisis ---------
if (compt_touche_int==0)nombre_saisi_int=0;   // si aucun chiffre saisi
if (compt_touche_int==1)nombre_saisi_int=saisie_int[0];   // si 1 seul chiffre saisi
if (compt_touche_int==2)nombre_saisi_int=(saisie_int[0]*10)+(saisie_int[1]*1);   // si 2 chiffres saisi
if (compt_touche_int==3)nombre_saisi_int=(saisie_int[0]*100)+(saisie_int[1]*10)+(saisie_int[2]*1);   // si 3 chiffres saisi
if (compt_touche_int==4)nombre_saisi_int=(saisie_int[0]*1000)+(saisie_int[1]*100)+(saisie_int[2]*10)+(saisie_int[3]*1);   // si 4 chiffres saisi
if (compt_touche_int==5)nombre_saisi_int=(saisie_int[0]*10000)+(saisie_int[1]*1000)+(saisie_int[2]*100)+(saisie_int[3]*10)+(saisie_int[4]*1);   // si 5 chiffres saisi

//------ à réaliser en sortie de lecture du clavier ----


compt_touche_int=0; // réinitialise le nombre de touche saisies

//----- renvoi de la valeur saisie ----

return (nombre_saisi_int); // renvoie la valeur - sortie de la fonction

}


void ReceptionSmS(){
  
     char c;
GSM_SMS sms;

  // If there are any SMSs available()
  if (sms.available()) {
    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    // An example of message disposal
    // Any messages starting with # should be discarded
    if (sms.peek() == '#') {
      Serial.println("Discarded SMS");
      sms.flush();
    }

    // Read message bytes and print them
    while (c = sms.read()) {
      Serial.print(c);
    }
  }

  delay(1000);
    }





void SMS()
{

Serial.println("AT+CMGF=1");    
delay(2000);
Serial.print("AT+CMGS=\"");
Serial.print(phone_no); 
Serial.write(0x22);
Serial.write(0x0D);  // hex equivalent of Carraige return    
Serial.write(0x0A);  // hex equivalent of newline
delay(2000);
Serial.print("alarme dans la maison");
delay(500);
Serial.println (char(26));//the ASCII code of the ctrl+z is 26
}


  void horloge()
{

DateTime now = RTC.now();
lcd.setCursor(0, 0);
if (now.day()<10)
{
 lcd.print("0");
}
lcd.print(now.day());
lcd.setCursor(2, 0);
lcd.print("/");
if (now.month()<10)
{
 lcd.print("0");
}
lcd.print(now.month());
lcd.setCursor(5, 0);
lcd.print("/");
lcd.setCursor(6, 0);
lcd.print(now.year());
lcd.setCursor(10, 0);
lcd.print(" ");
lcd.setCursor(11, 0);
if (now.hour()<10)
{
 lcd.print("0");
}
lcd.print(now.hour());
lcd.setCursor(13, 0);
lcd.print(":");
lcd.setCursor(14, 0);
if (now.minute() < 10) 
{
lcd.print("0");
}
lcd.print(now.minute());
}


// ////////////////////////// Fin du programme //////////////////// 
