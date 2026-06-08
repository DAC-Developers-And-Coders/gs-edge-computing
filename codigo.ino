#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define lvd 3
#define lam 4
#define lvm 5

#define btt 2

#define intensidade A0
#define densidade A1
#define velocidade A2

LiquidCrystal_I2C lcd(0x20,16,2);

int intenvalue;
int denvalue;
int velovalue;
float estabilidade;
int tela;
int state_bt;
int state_ant;
int alert;

void setup()
{
  pinMode(lvd, OUTPUT);
  pinMode(lam, OUTPUT);
  pinMode(lvm, OUTPUT);

  pinMode(btt, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop()
{
  state_bt = digitalRead(btt);
  
  calc_estabilidade();
  
  digitalWrite(lvd, alert == 1);
  digitalWrite(lam, alert == 2);
  digitalWrite(lvm, alert == 3);
  
  if (state_bt == 0 && state_ant == 1){
   tela++;
   lcd.clear();
   delay(50);
  }
 
  if (tela > 3){
    tela = 0;
  }
  
  selet();
  state_ant = state_bt;
}


void calc_estabilidade(){
  intenvalue = map(analogRead(intensidade), 0, 1023, 0, 100);
  denvalue = map(analogRead(densidade), 0, 1023, 0, 100);
  velovalue = map(analogRead(velocidade), 0, 1023, 0, 100);
  
  estabilidade = intenvalue / (1 + 0.7 * (denvalue / 100.0) + 0.4 * (velovalue / 100.0));
  
  if (estabilidade < 35){alert = 3;}
  else if(estabilidade < 50){alert =2;}
  else{alert = 1;}
} 

void selet(){
  switch(tela){
    case 0:
    tela_estab();
    break;
    case 1:
    tela_inte();
    break;
    case 2:
    tela_den();
    break;
    case 3:
    tela_velo();
    break;
  }   
}
void tela_estab(){
  lcd.setCursor(0,0);
  lcd.print("Estabilidade");
  lcd.setCursor(0,1);
  lcd.print(estabilidade,1);
}

void tela_inte(){
  lcd.setCursor(0,0);
  lcd.print("Intensidade");
  lcd.setCursor(0,1);
  lcd.print(intenvalue);
  lcd.print("%");
}

void tela_den(){
  lcd.setCursor(0,0);
  lcd.print("Densidade");
  lcd.setCursor(0,1);
  lcd.print(denvalue);
  lcd.print("%");
}

void tela_velo(){
  lcd.setCursor(0,0);
  lcd.print("Velocidade");
  lcd.setCursor(0,1);
  lcd.print(velovalue);
  lcd.print("%");
}