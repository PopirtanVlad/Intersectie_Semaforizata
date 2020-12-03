//Denumiri
#define GREEN 2
#define YELLOW 1
#define RED 0

//Pini
#define CARS_RED 13
#define CARS_YELLOW 12
#define CARS_GREEN 11
#define CARS_INTERMITENT_GREEN 10
#define HUMAN_RED 4
#define HUMAN_GREEN 5
#define BUTTON_1_PEDESTRIAN 2
#define BUTTON_2_PEDESTRIAN 3
#define BUTTON_3_PEDESTRIAN 20
#define BUTTON_4_PEDESTRIAN 21
struct semaphoreCars{
  boolean r; //red
  boolean g; //green
  boolean y; //yellow. This will always be turned off for pedestrians  
  boolean intG; //intermitent
};
struct semaphorePed{
  boolean r; //red
  boolean g; //green
};

//Variabile de timp
int interval=200;
unsigned long previousMillis=0;

//Variabile semafoare
semaphoreCars semCar[4]={{true,false,false,false},{true,false,false,false},{true,false,false,false},{true,false,false,false}};
semaphorePed semPed[4]={{false,true},{false,true},{false,true},{false,true}};
volatile boolean pedestrianWaiting[4]={false,false,false,false};

void setup()
{
  Serial.begin(9600);
  pinMode(CARS_RED, OUTPUT);
  pinMode(CARS_YELLOW, OUTPUT);
  pinMode(CARS_GREEN, OUTPUT);
  pinMode(CARS_INTERMITENT_GREEN, OUTPUT);
  pinMode(HUMAN_RED, OUTPUT);
  pinMode(HUMAN_GREEN, OUTPUT);
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
    digitalWrite(CARS_RED,semCar[0].r);
    digitalWrite(CARS_YELLOW,semCar[0].y);
    digitalWrite(CARS_GREEN,semCar[0].g);
    intermitentBlink();
  
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
    digitalWrite(10, semCar[0].intG);
    /*digitalWrite(9,semCar[1].intG);
    digitalWrite(8,semCar[2].intG);
    digitalWrite(7,semCar[3].intG);*/ //Pini provizorii daca puneam intr-un for incercau sa scrie pe acelasi pin si era conflict
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
       pedestrianWaiting[0]=true;
  }
   last_millis = millis();
}


void debounceButton2(){
  static unsigned long debouncing_time = 100;
  static unsigned long last_millis = millis();
  
  if((millis() - last_millis) >= debouncing_time)
  {
       pedestrianWaiting[1]=true;
  }
   last_millis = millis();
}


void debounceButton3(){
  static unsigned long debouncing_time = 100;
  static unsigned long last_millis = millis();
  
  if((millis() - last_millis) >= debouncing_time)
  {
       pedestrianWaiting[2]=true;
  }
   last_millis = millis();
}

void debounceButton4(){
  static unsigned long debouncing_time = 100;
  static unsigned long last_millis = millis();
  
  if((millis() - last_millis) >= debouncing_time)
  {
       pedestrianWaiting[3]=true;
  }
   last_millis = millis();
}
