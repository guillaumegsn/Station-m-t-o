int data=4;
int T=50;
int i;
int lect;
int a=0;
int b=0;
int tab[50];
int puiss=1;
char caract;
int caractnum;
//int Letra[40]
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
  // put your setup code here, to run once:
  lcd.init();  // initialize the lcd 
  lcd.backlight(); // lumier on
  Serial.begin(250000);
  pinMode(data,INPUT);
  
}

void loop() {
  a=0;
  puiss=1;
  caract =0;
  caractnum=0;


  lect=digitalRead(data);
  //Serial.print(lect);
  if(lect==LOW){
    delay(T/4);

    a++;

    
    delay(T);
    lect=digitalRead(data);

    if(lect==LOW)a++;

    delay(T);
    lect=digitalRead(data);

    if(lect==HIGH)a++;

    
    if(a==3){
    
      delay(T);
      for(i=0;i<8;i++){
        lect=digitalRead(data);
        if(lect==HIGH)tab[i]=1;
        if(lect==LOW)tab[i]=0;
        delay(T);
      }

        //for(i=0;i<8;i++){
          //if(i==4)Serial.print(" ");
          //Serial.print(tab[i]);

      //}
      //Serial.print(" ");
      //Serial.println("---------------------");
      delay(T);

      for(b=7;b>0;b--){
        caractnum=caractnum+tab[b]*puiss;
        caract=caract+tab[b]*puiss;
        puiss= puiss*2;
      }
      
      //Serial.println("votre caratere est: ");
      //Serial.print(caractnum);

      //Serial.print("  ==>  ");
      //Serial.println(caract);
      
      
      if(caract=='@'){
        Serial.println(" ");
        lcd.setCursor(0, 1);
      }
      else{
        if(caract=='#'){
          lcd.clear(); 
          Serial.println(" ");
          Serial.println("---------------");
          Serial.println(" ");
          Serial.println(" ");


        }
        else{
          Serial.print(caract);
          lcd.print(caract);
        }
      }
    }
  } 
}
