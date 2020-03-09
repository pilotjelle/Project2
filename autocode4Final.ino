#define linksVooruit 10
#define linksAchteruit 11

#define rechtsVooruit 12
#define rechtsAchteruit 13

#define ultrasoonRechtsTrig 5
#define ultrasoonRechtsEcho 6
#define ultrasoonVoorTrig 3
#define ultrasoonVoorEcho 4
#define ultrasoonLinksTrig 8
#define ultrasoonLinksEcho 9
#define knop 2
//#define reed 7

  const int linksIR= A1;
  const int rechtsIR= A2;
  int time;
  int distance1;
  int distance2;
  int distance3;

  bool objectL= false;
  bool objectR= false;
  int esp = 0;
  int data=0;
  




void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
 // pinMode(7, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(linksIR, INPUT);
  pinMode(rechtsIR, INPUT);
  Serial.begin(115200);
  
  
}

void loop() {
int detectL = digitalRead(linksIR);
    int detectR = digitalRead(rechtsIR);
  Serial.println(digitalRead(knop));
if(digitalRead(2)== HIGH){ // kijkt of de robot autonoom moet rijden of handmatig via een switch
  esp=0;
        
  // ultrasoon naar rechts
          digitalWrite (ultrasoonRechtsTrig, HIGH);
          delayMicroseconds (10);
          digitalWrite (ultrasoonRechtsTrig, LOW);
          time = pulseIn (ultrasoonRechtsEcho, HIGH);
          distance1 = (time * 0.034) / 2;
      
         //ultrasoon naar voren 
          digitalWrite (ultrasoonVoorTrig, HIGH);
          delayMicroseconds (10);
          digitalWrite (ultrasoonVoorTrig, LOW);
          time = pulseIn (ultrasoonVoorEcho, HIGH);
          distance2 = (time * 0.034) / 2;
  
          // ultrasoon naar links
          digitalWrite (ultrasoonLinksTrig, HIGH);
          delayMicroseconds (10);
          digitalWrite (ultrasoonLinksTrig, LOW);
          time = pulseIn (ultrasoonLinksEcho, HIGH);
          distance3 = (time * 0.034) / 2;
        // voor de lijn en afgrond 
// if(digitalRead(7)==LOW){
//  Stop();
//}else 
if (detectR == 1 && detectL == 1) { // als beide ir-sensoren de rivier of een afgrond detecteren
        Achteruit(); 
        delay(400);
        Links();
        delay(400);
      } else if (detectL == 1 && detectR == 0) { // als de linker ir-sensor een rivier of afgrond detecteerd
    
      Rechts();
      }
      else if (detectR == 1 && detectL == 0) {   // als de rechter ir-sensor een rivier of afgrond detecteerd
        Links();
      }
      
      else if(distance2<10 && distance1<5) // als er een obstakel in de weg staat
        {
          Links();
        } else if(distance3<5 && distance2<10) // als er een obtakel in de weg staat
        {
          Rechts();
        } else if(distance2<10 && distance2>0) // als de ultrasoon sensor in het midden iets detecteert binnen 10cm
        {
          Rechts();
        } else if(distance1<5)  // als de rechter ultrasoon sensor iets detecteert binnen 5cm
        {
          Links();
        } else if(distance3<5) // als de linker ultrasoon sensor iets detecteert binnen 5cm
        {
          Rechts();
        } else if(distance1<5 && distance3<5) // als de linker en de rechter ultrasoon sensoren iets binnen 5cm detecteren
        {
          Achteruit();
          delay(300);
          Rechts();
          delay(900);
        } else 
        {
          Vooruit();
        }
  
  }else if(esp==0){ // als de switch naar handmatig wordt gezet stopt de robot en wacht hij op verdere instructies
    Stop();
    esp=1;
  }else
  {
    if (Serial.available()>0){ 
  data=Serial.read();
  Serial.println(data);
    }
    if(data==1){ // als 1(Vooruit) wordt verstuurd via de esp gaat de robot vooruit
    Vooruit();
  } else if(data==2){ // als 2(Achteruit) wordt verstuurd via de esp gaat de robot vooruit
    Achteruit();
  } else if(data==3){ // als 3(Rechts) wordt verstuurd via de esp gaat de robot vooruit
    Rechts();
  } else if(data==4){ // als 4(Links) wordt verstuurd via de esp gaat de robot vooruit
    Links();
  } else if(data==0){ // als 0(stop) wordt verstuurd via de esp gaat de robot vooruit
    Stop();
  }

}
}

void Vooruit() {
  digitalWrite(linksVooruit, HIGH);
  digitalWrite(rechtsVooruit, HIGH);
  digitalWrite(linksAchteruit, LOW);
  digitalWrite(rechtsAchteruit, LOW);

}
void Achteruit() {
  digitalWrite(linksAchteruit, HIGH);
  digitalWrite(rechtsAchteruit, HIGH);
  digitalWrite(linksVooruit, LOW);
  digitalWrite(rechtsVooruit, LOW);

}
void Stop() {
  digitalWrite(linksAchteruit, LOW);
  digitalWrite(rechtsAchteruit, LOW);
  digitalWrite(linksVooruit, LOW);
  digitalWrite(rechtsVooruit, LOW);

}

void Links() {
  
  digitalWrite(rechtsVooruit, HIGH);
  digitalWrite(linksAchteruit, HIGH);
  digitalWrite(linksVooruit, LOW);
  digitalWrite(rechtsAchteruit, LOW);

}
void Rechts() {
  
  digitalWrite(rechtsVooruit, LOW);
  digitalWrite(linksAchteruit, LOW);
  digitalWrite(linksVooruit, HIGH);
  digitalWrite(rechtsAchteruit, HIGH);

}
