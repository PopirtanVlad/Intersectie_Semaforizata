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

#define BUTTON_1_PEDESTRIAN 20
#define BUTTON_2_PEDESTRIAN 3
#define BUTTON_3_PEDESTRIAN 2
#define BUTTON_4_PEDESTRIAN 21
struct semaphoreCars{
  boolean r; //red
  boolean g; //green
  int senzorIR;
};
struct semaphorePed{
  boolean g; //green
};

//Structura om
struct pedestrianStruct{
  volatile boolean waiting;
  volatile int senzor;  
};

//Variabile de timp
int interval=200;
unsigned long previousMillis=0;
int counterPedButton;

//Variabile semafoare
semaphoreCars semCar[4]={{false,true,0},{true,false,0},{true,false,0},{true,false,0}};
semaphorePed semPed[4]={{true},{false},{false},{false}};
volatile boolean pedestrianWaiting[4]={false,false,false,false};


//Variabile oameni
pedestrianStruct pedestrian[4]={{false,0},{false,0},{false,0},{false,0}};

void setup()
{
  Serial.begin(9600);
  Timer1.initialize(500000);
  Timer1.attachInterrupt(intermitent);
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
  pinMode(9,OUTPUT);
  pinMode(BUTTON_1_PEDESTRIAN, INPUT_PULLUP);
  pinMode(BUTTON_2_PEDESTRIAN, INPUT_PULLUP);
  pinMode(BUTTON_3_PEDESTRIAN, INPUT_PULLUP);
  pinMode(BUTTON_4_PEDESTRIAN, INPUT_PULLUP);
  attachInterrupt(BUTTON_1_PEDESTRIAN,debounceButton1,RISING);
  attachInterrupt(BUTTON_2_PEDESTRIAN,debounceButton2,RISING);
  attachInterrupt(BUTTON_3_PEDESTRIAN,debounceButton3,RISING);
  attachInterrupt(BUTTON_4_PEDESTRIAN,debounceButton4,RISING);
  Serial.begin(9600);
}

void loop(){
  checkStreetTraffic();
  readSensors();
  resolve_pedestrian();
  writeOnLeds();
}

void writeOnLeds(){
   digitalWrite(41,semCar[0].r);digitalWrite(40,semCar[0].g);digitalWrite(23,semPed[0].g);
   digitalWrite(50,semCar[1].r);digitalWrite(51,semCar[1].g);digitalWrite(24,semPed[1].g);
   digitalWrite(53,semCar[2].r);digitalWrite(52,semCar[2].g);digitalWrite(22,semPed[2].g);
   digitalWrite(42,semCar[3].r);digitalWrite(43,semCar[3].g);digitalWrite(25,semPed[3].g);
}

void intermitent(){
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

void debounceButton1(){
  static unsigned long debouncing_time = 100;
  static unsigned long last_millis = millis();
  
  if((millis() - last_millis) >= debouncing_time)
  {    
       //while(semCar[0].g!=true){
            pedestrian[0].waiting=true;
       //}
  }
   last_millis = millis();
}


void debounceButton2(){
  static unsigned long debouncing_time = 100;
  static unsigned long last_millis = millis();
  
  if((millis() - last_millis) >= debouncing_time)
  {
       pedestrian[1].waiting=true;
  }
   last_millis = millis();
}


void debounceButton3(){
  static unsigned long debouncing_time = 100;
  static unsigned long last_millis = millis();
  
  if((millis() - last_millis) >= debouncing_time)
  {
      pedestrian[2].waiting=true;
  }
   last_millis = millis();
}

void debounceButton4(){
  static unsigned long debouncing_time = 100;
  static unsigned long last_millis = millis();
  
  if((millis() - last_millis) >= debouncing_time)
  {
       pedestrian[3].waiting=true;
  }
   last_millis = millis();
}



void checkStreetTraffic(){
  if(semCar[0].senzorIR>500){
    int ok=1; //Verificam daca nu e deja un semafor verde.
    for(int i=0;i<3;i++){
      if(i!=0 && semCar[i].senzorIR>500){
        ok=0;
        break;
      }
      if(pedestrian[i].waiting==true){
        ok=0;
        break;
      }
    }
    if (ok==1 && semCar[0].r==true){
      semCars(0, 1);
      semCars(1, 0);
      semCars(2, 0);
      semCars(3, 0);
    }
  }
 if(semCar[1].senzorIR>500){
    int ok=1; //Verificam daca nu e deja un semafor verde.
    for(int i=0;i<3;i++){
      if(i!=1 && semCar[i].senzorIR>500){
        ok=0;
        break;
      }
      if(pedestrian[i].waiting==true){
        ok=0;
        break;
      }
    }
    if (ok==1 && semCar[1].r==true){
      semCars(1, 1);
      semCars(2, 0);
      semCars(0, 0);
      semCars(3, 0);
    }
  }
  if(semCar[2].senzorIR>500){
    int ok=1; //Verificam daca nu e deja un semafor verde.
    for(int i=0;i<3;i++){
      if(i!=2 && semCar[i].senzorIR>500){
        ok=0;
        break;
      }
      if(pedestrian[i].waiting==true){
        ok=0;
        break;
      }
    }
    if (ok==1 && semCar[2].r==true){
      semCars(2, 1);
      semCars(1, 0);
      semCars(0, 0);
      semCars(3, 0);
    }
  }
  if(semCar[3].senzorIR>500){
    int ok=1; //Verificam daca nu e deja un semafor verde.
    for(int i=0;i<3;i++){
      if(i!=3 && semCar[i].senzorIR>500){
        ok=0;
        break;
      }
      if(pedestrian[i].waiting==true){
        ok=0;
        break;
      }
    }
    if (ok==1 && semCar[3].r==true){
      semCars(3, 1);
      semCars(0, 0);
      semCars(1, 0);
      semCars(2, 0);
    }
  }
}

void readSensors(){
  semCar[0].senzorIR=analogRead(IR_1);
  semCar[1].senzorIR=analogRead(IR_2);
  semCar[2].senzorIR=analogRead(IR_3);
  semCar[3].senzorIR=analogRead(IR_4); 
}


void semPedestrians(int whichPed){
  /*boolean greenValue[4]={false,false,false,false};
  for(int i=0;i<4;i++){
    greenValue[4]=semCar[i].g;
    if(i==whichPed){
      semCars(i,RED);
      semPed[i].g=true;
    }
    else{
      semCars(i,GREEN);
      semPed[i].g=false;
    }
  }
  //delay(1000);
  for(int i=0;i<4;i++){
    semCar[i].g=greenValue[i];
    semCar[i].r=not greenValue[i];
  }
  pedestrian[whichPed].waiting=false;*/
}

void resolve_pedestrian(){
  if(pedestrian[0].waiting && semCar[0].g==false){
    semPedestrians(0);
    return;
  }
  if(pedestrian[1].waiting && semCar[1].g==false){
     semPedestrians(1);
     return;
  }
  if(pedestrian[2].waiting && semCar[2].g==false){
     semPedestrians(2);
     return;
  }
  if(pedestrian[3].waiting && semCar[3].g==false){
     semPedestrians(3);
  }
}
