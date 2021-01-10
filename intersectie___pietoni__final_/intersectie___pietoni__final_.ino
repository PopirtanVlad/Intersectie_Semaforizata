#include "TimerOne.h" 

//Denumiri
#define GREEN 1
#define RED 0

//Pini
#define IR_1 A13
#define IR_2 A14
#define IR_3 A0
#define IR_4 A1

#define CARS1_RED 41
#define CARS1_GREEN 40
#define HUMAN1_GREEN 23

#define CARS2_RED 51
#define CARS2_GREEN 50
#define HUMAN2_GREEN 24

#define CARS3_RED 53
#define CARS3_GREEN 52
#define HUMAN3_GREEN 22

#define CARS4_RED 43
#define CARS4_GREEN 42
#define HUMAN4_GREEN 25

#define BUTTON_1_PEDESTRIAN 21
#define BUTTON_2_PEDESTRIAN 3
#define BUTTON_3_PEDESTRIAN 2
#define BUTTON_4_PEDESTRIAN 20
struct semaphoreCars{
  boolean r; //red
  boolean g; //green
  int senzorIR;
};
struct semaphorePed{
  boolean g; //green
};

//Structura om
/*struct pedestrianStruct{
  volatile boolean waiting;
  volatile int senzor;  
};*/
//Variabile de timp
volatile int pedestrian1Counter=false;

//Variabile semafoare
semaphoreCars semCar[4]={{false,true,0},{true,false,0},{true,false,0},{true,false,0}};
semaphorePed semPed[4]={{false},{false},{false},{false}};
boolean prevGreens[4]={false,false,false,false};



void setup()
{
  //Timer1.initialize(500000);
  //Timer1.attachInterrupt(intermitent);
  pinMode(IR_1,INPUT);
  pinMode(IR_2,INPUT);
  pinMode(IR_3,INPUT);
  pinMode(IR_4,INPUT);
  pinMode(CARS1_RED, OUTPUT);
  pinMode(CARS1_GREEN, OUTPUT);
  pinMode(HUMAN1_GREEN, OUTPUT);
  pinMode(CARS2_RED, OUTPUT);
  pinMode(CARS2_GREEN, OUTPUT);
  pinMode(HUMAN2_GREEN, OUTPUT);
  pinMode(CARS3_RED, OUTPUT);
  pinMode(CARS3_GREEN, OUTPUT);
  pinMode(HUMAN3_GREEN, OUTPUT);
  pinMode(CARS4_RED, OUTPUT);
  pinMode(CARS4_GREEN, OUTPUT);
  pinMode(HUMAN4_GREEN, OUTPUT);
  pinMode(BUTTON_1_PEDESTRIAN, INPUT_PULLUP);
  pinMode(BUTTON_2_PEDESTRIAN, INPUT_PULLUP);
  pinMode(BUTTON_3_PEDESTRIAN, INPUT_PULLUP);
  pinMode(BUTTON_4_PEDESTRIAN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1_PEDESTRIAN),buttonPressed,FALLING);
  Serial.begin(9600);
}

void loop(){
  //Daca pedestrian1Counter are valoarea 'true', inseamna ca ne aflam in intervalul de 4 secunde in care un pieton poate trece strada.
  //Daca un pieton poate trece strada, semaforul pentru masini nu trebuie sa permita culoarea verde, pentru a evita accidentele.
  if(pedestrian1Counter==false){
    checkStreetTraffic();
    readSensors();
  }
  writeOnLeds();
}

void writeOnLeds(){
   digitalWrite(41,semCar[0].r);digitalWrite(40,semCar[0].g);
   digitalWrite(50,semCar[1].r);digitalWrite(51,semCar[1].g);
   digitalWrite(53,semCar[2].r);digitalWrite(52,semCar[2].g);
   digitalWrite(42,semCar[3].r);digitalWrite(43,semCar[3].g);
}

void intermitent(){
  //Pentru fiecare semafor verificam daca este activ in starea rosu. Daca da, negam valoarea.
  for(int i=0;i<4;i++){
    if (semCar[i].r==true){
      if(semCar[i].g==false){
         semCar[i].g = true;
      } else {
         semCar[i].g = false;
      }
    }
  }
}

void semCars(int whichSem, int whichColor){//0=red 1=green
  //whichSem decide ce semafor isi schimba culoarea, iar whichColor decide culoarea pe care o dorim.
  switch(whichColor){
    case 0:
      semCar[whichSem].r=true;
      semCar[whichSem].g=false;
      break;
    case 1:
      semCar[whichSem].r=false;
      semCar[whichSem].g=true;
      break;
  }
}

void buttonPressed(){
   //Verificam daca butonul nu a fost deja apasat. Daca este prima oara in intervalul de 4 secunde, salvam culorile precedente ale semafoarelor si asignam valoarea true variabilei de test.
   if(pedestrian1Counter==false){
      
      pedestrian1Counter=true;
      for(int i=0;i<4;i++){
        prevGreens[i]=semCar[i].g;
     }
     //Aprindem ledurile pentru pietoni de pe sensurile opuse si stingem/aprindem semafoarele masinilor in functie de pietoni.
     digitalWrite(HUMAN1_GREEN,HIGH);
     digitalWrite(HUMAN3_GREEN,HIGH);
     semCars(0,RED);
     semCars(1,GREEN);
     semCars(2,RED);
     semCars(3,GREEN);
     //Dezactivam intreruperile globale
     cli(); 
     //Setam valoarea timerelor la 0
     TCCR1A = 0; 
     TCCR1B = 0;
     // setăm registrul cu valoarea căruia vom compara TCNT
     OCR1A = 64624;
     // activăm modul CTC:
     TCCR1B |= (1 << WGM12);
     TCCR1B |= (1 << CS10);
     TCCR1B |= (1 << CS12);
     // facem enable la întreruperea la comparare prin setarea bitului corespunzător din mască
     TIMSK1 |= (1 << OCIE1A);
     //Reactivam interuperile globale
     sei();
    }
}

//Rutina de intreruperi. Aceasta functie este apelata atunci cand counterul intern depaseste valoarea salvata in registrul de comparare.
ISR(TIMER1_COMPA_vect){
   //Pentru a nu se repeta acest bloc de instructiuni la fiecare 4 secunde pentru o perioada nelimitata de timp, vom folosi variabila pedestrian1Counter.
   if(pedestrian1Counter==true){
    //Daca aceasta a fost activata precedent de catre functia intreruperii, stingem ledurile pietonilor si dam semafoarelor variabilele dinainte de a avea loc intreruperea.
     digitalWrite(HUMAN1_GREEN,LOW);
     digitalWrite(HUMAN3_GREEN,LOW);
     for(int i=0;i<4;i++){
        semCar[i].g=prevGreens[i];
        semCar[i].r=not prevGreens[i];
     }
   }
   //Ne asiguram ca variabila de test trece la valoarea false.
   pedestrian1Counter=false;
}


//Functie ce verifica starea traficului
void checkStreetTraffic(){
  //Daca senzorul semaforului respectiv, citeste o luminozitate scazuta, inseamna ca masina se afla in fata senzorului.
  //Daca aceasta conditie este indeplinita, impreuna cu valoarea 'false' a variabilei de conditie pentru semaforul pietonilor
  //verificam daca exista alt senzor ce intercepteaza o masina in fata sa. Daca da, ne asiguram ca nu se schimba culoarea.
  //Daca functia trece de ambele conditii si culoarea actuala este rosie(conditie adaugata pentru intermitent), culoarea semaforului va deveni verde, iar celalalte se vor schimba in rosu.
  if(semCar[0].senzorIR>500 && pedestrian1Counter==false){
    int ok=1; //Verificam daca nu e deja un semafor verde.
    for(int i=0;i<3;i++){
      if(i!=0 && semCar[i].senzorIR>500){
        ok=0;
        break;
      }
    }
    if (ok==1 && semCar[0].r==true){
      semCars(0, GREEN);
      semCars(1, RED);
      semCars(2, RED);
      semCars(3, RED);
    }
  }
 if(semCar[1].senzorIR>500){
    int ok=1; //Verificam daca nu e deja un semafor verde.
    for(int i=0;i<3;i++){
      if(i!=1 && semCar[i].senzorIR>500){
        ok=0;
        break;
      }
    }
    if (ok==1 && semCar[1].r==true){
      semCars(1, GREEN);
      semCars(2, RED);
      semCars(0, RED);
      semCars(3, RED);
    }
  }
  if(semCar[2].senzorIR>500){
    int ok=1; //Verificam daca nu e deja un semafor verde.
    for(int i=0;i<3;i++){
      if(i!=2 && semCar[i].senzorIR>500){
        ok=0;
        break;
      }
    if (ok==1 && semCar[2].r==true){
      semCars(2, GREEN);
      semCars(1, RED);
      semCars(0, RED);
      semCars(3, RED);
    }
  }
 }
  if(semCar[3].senzorIR>500 && pedestrian1Counter==false){
    int ok=1; //Verificam daca nu e deja un semafor verde.
    for(int i=0;i<3;i++){
      if(i!=3 && semCar[i].senzorIR>500){
        ok=0;
        break;
      }
    }
    if (ok==1 && semCar[3].r==true){
      semCars(3, GREEN);
      semCars(0, RED);
      semCars(1, RED);
      semCars(2, RED);
    }
  }
}
 

void readSensors(){
  //Citim cei 4 senzori.
  semCar[0].senzorIR=analogRead(IR_1);
  semCar[1].senzorIR=analogRead(IR_2);
  semCar[2].senzorIR=analogRead(IR_3);
  semCar[3].senzorIR=analogRead(IR_4); 
}
