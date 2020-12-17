//Denumiri
#define GREEN 2
#define YELLOW 1
#define RED 0

//Pini
#define IR_1 A0
#define IR_2 A1
#define IR_3 A2
#define IR_4 A3
#define PIR_1 A4 
#define PIR_2 A5
#define PIR_3 A6
#define PIR_4 A7

#define CARS1_RED 22
#define CARS1_YELLOW 23
#define CARS1_GREEN 24
#define CARS1_INTERMITENT_GREEN 25
#define HUMAN1_RED 26
#define HUMAN1_GREEN 27

#define CARS2_RED 28
#define CARS2_YELLOW 29
#define CARS2_GREEN 30
#define CARS2_INTERMITENT_GREEN 31
#define HUMAN2_RED 32
#define HUMAN2_GREEN 33

#define CARS3_RED 34
#define CARS3_YELLOW 35
#define CARS3_GREEN 36
#define CARS3_INTERMITENT_GREEN 37
#define HUMAN3_RED 38
#define HUMAN3_GREEN 39

#define CARS4_RED 40
#define CARS4_YELLOW 41
#define CARS4_GREEN 42
#define CARS4_INTERMITENT_GREEN 43
#define HUMAN4_RED 44
#define HUMAN4_GREEN 45

#define BUTTON_1_PEDESTRIAN 2
#define BUTTON_2_PEDESTRIAN 3
#define BUTTON_3_PEDESTRIAN 20
#define BUTTON_4_PEDESTRIAN 21
struct semaphoreCars{
  boolean r; //red
  boolean g; //green
  boolean y; //yellow. This will always be turned off for pedestrians  
  boolean intG; //intermitent
  int senzorIR;
};
struct semaphorePed{
  boolean r; //red
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
semaphoreCars semCar[4]={{true,false,false,false,0},{true,false,false,false,0},{true,false,false,false,0},{true,false,false,false,0}};
semaphorePed semPed[4]={{false,true},{false,true},{false,true},{false,true}};
volatile boolean pedestrianWaiting[4]={false,false,false,false};


//Variabile oameni
pedestrianStruct pedestrian[4]={{false,0},{false,0},{false,0},{false,0}};

void setup()
{
  Serial.begin(9600);
  pinMode(PIR_1,INPUT);
  pinMode(PIR_2,INPUT);
  pinMode(PIR_3,INPUT);
  pinMode(PIR_4,INPUT);
  pinMode(IR_1,INPUT);
  pinMode(IR_2,INPUT);
  pinMode(IR_3,INPUT);
  pinMode(IR_4,INPUT);
  pinMode(CARS1_RED, OUTPUT);
  pinMode(CARS1_YELLOW, OUTPUT);
  pinMode(CARS1_GREEN, OUTPUT);
  pinMode(CARS1_INTERMITENT_GREEN, OUTPUT);
  pinMode(HUMAN1_RED, OUTPUT);
  pinMode(HUMAN1_GREEN, OUTPUT);
  pinMode(CARS2_RED, OUTPUT);
  pinMode(CARS2_YELLOW, OUTPUT);
  pinMode(CARS2_GREEN, OUTPUT);
  pinMode(CARS2_INTERMITENT_GREEN, OUTPUT);
  pinMode(HUMAN2_RED, OUTPUT);
  pinMode(HUMAN2_GREEN, OUTPUT);
  pinMode(CARS3_RED, OUTPUT);
  pinMode(CARS3_YELLOW, OUTPUT);
  pinMode(CARS3_GREEN, OUTPUT);
  pinMode(CARS3_INTERMITENT_GREEN, OUTPUT);
  pinMode(HUMAN3_RED, OUTPUT);
  pinMode(HUMAN3_GREEN, OUTPUT);
  pinMode(CARS4_RED, OUTPUT);
  pinMode(CARS4_YELLOW, OUTPUT);
  pinMode(CARS4_GREEN, OUTPUT);
  pinMode(CARS4_INTERMITENT_GREEN, OUTPUT);
  pinMode(HUMAN4_RED, OUTPUT);
  pinMode(HUMAN4_GREEN, OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(BUTTON_1_PEDESTRIAN, INPUT);
  attachInterrupt(BUTTON_1_PEDESTRIAN,debounceButton1,FALLING);
  attachInterrupt(BUTTON_2_PEDESTRIAN,debounceButton2,FALLING);
  attachInterrupt(BUTTON_3_PEDESTRIAN,debounceButton3,FALLING);
  attachInterrupt(BUTTON_4_PEDESTRIAN,debounceButton4,FALLING);
}

void loop(){
    semCars(0,RED);
    semCars(1,GREEN);
    writeOnLeds();
    intermitentBlink();
  
}

void writeOnLeds(){
   digitalWrite(22,semCar[0].r);digitalWrite(23,semCar[0].y);digitalWrite(24,semCar[0].g);digitalWrite(26,semPed[0].r);digitalWrite(27,semPed[0].g);
   digitalWrite(28,semCar[0].r);digitalWrite(29,semCar[0].y);digitalWrite(30,semCar[0].g);digitalWrite(32,semPed[0].r);digitalWrite(33,semPed[0].g);
   digitalWrite(34,semCar[0].r);digitalWrite(35,semCar[0].y);digitalWrite(36,semCar[0].g);digitalWrite(38,semPed[0].r);digitalWrite(39,semPed[0].g);
   digitalWrite(40,semCar[0].r);digitalWrite(41,semCar[0].y);digitalWrite(42,semCar[0].g);digitalWrite(44,semPed[0].r);digitalWrite(45,semPed[0].g);
}

void intermitentBlink(){
  unsigned long currentMillis=millis();
  for(int i=0;i<4;i++){
    if(semCar[i].g==0){
      if (currentMillis - previousMillis >= interval) {
        previousMillis = millis();
        if (semCar[i].intG == false) {
          semCar[i].intG = true;
        } else {
          semCar[i].intG = false;
        }
      }
    }
    else{  
      semCar[i].intG=true;
    }
    digitalWrite(CARS1_INTERMITENT_GREEN,semCar[0].intG);
    digitalWrite(CARS2_INTERMITENT_GREEN,semCar[1].intG);
    digitalWrite(CARS3_INTERMITENT_GREEN,semCar[2].intG);
    digitalWrite(CARS4_INTERMITENT_GREEN,semCar[3].intG); 
  }
}

void semCars(int whichSem, int whichColor){//1=red 2=yellow 3=green
  switch(whichColor){
    case 0:
      semCar[whichSem].r=true;
      semCar[whichSem].g=false;
      semCar[whichSem].y=false;
      break;
    case 1:
      semCar[whichSem].r=false;
      semCar[whichSem].g=false;
      semCar[whichSem].y=true;
      break;
    case 2:
      semCar[whichSem].r=false;
      semCar[whichSem].g=true;
      semCar[whichSem].y=false;
      break;
  }
}

void debounceButton1(){
  static unsigned long debouncing_time = 100;
  static unsigned long last_millis = millis();
  
  if((millis() - last_millis) >= debouncing_time)
  {
       pedestrian[0].waiting=true;
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
  bool firstTime[4]={true,true,true,true};
  while(semCar[0].senzorIR<300){
    /*functie de checkWhichGreen
     * if(firstTime[greenSem]){
     *  semCars(greenSem,1);
        Asteapta 2 secunde cu semaforul pe galben
      }
    */
    semCars(0, 2);
    semCars(1, 0);
    semCars(2, 0);
    semCars(3, 0);
    /*timer in care asteapta 1 secunda intre masini
    while(count>0){
      count--;
    }
    */
  }
  if (semCar[1].senzorIR<300){
    //Temporizare cu intreruperi
    semCars(1, 2);
    semCars(0, 0);
    semCars(2, 0);
    semCars(3, 0);
  }
  if (semCar[2].senzorIR<300){
    //Temporizare cu intreruperi
    semCars(2, 2);
    semCars(1, 0);
    semCars(0, 0);
    semCars(3, 0);
  }
  if (semCar[3].senzorIR<300){
    //Temporizare cu intreruperi
    semCars(3, 2);
    semCars(0, 0);
    semCars(1, 0);
    semCars(2, 0);
  }
}

void readSensors(){
  pedestrian[0].senzor=analogRead(PIR_1);
  pedestrian[1].senzor=analogRead(PIR_2);
  pedestrian[2].senzor=analogRead(PIR_3);
  pedestrian[3].senzor=analogRead(PIR_4); 
  semCar[0].senzorIR=analogRead(IR_1);
  semCar[1].senzorIR=analogRead(IR_2);
  semCar[2].senzorIR=analogRead(IR_3);
  semCar[3].senzorIR=analogRead(IR_4); 
}

void semPedestrians(int whichSem){
  if(TCNT1==0){
    while(TCNT1<2){
      semCars(whichSem,1);      
    }
  }
  else{
    while(TCNT2<2){
      semCars(whichSem,1);
    }
  }
  if(pedestrian[whichSem].senzor!=0){
    semPed[whichSem].r=false; semPed[whichSem].g=true;
  }
  else{
    semPed[whichSem].r=true; semPed[whichSem].g=false;
  }
}

void resolve_pedestrian(){
  if(pedestrian[0].waiting){
    semPedestrians(0);
  }
  if(pedestrian[1].waiting){
     semPedestrians(1);
  }
  if(pedestrian[2].waiting){
     semPedestrians(2);
  }
  if(pedestrian[3].waiting){
     semPedestrians(3);
  }
}
