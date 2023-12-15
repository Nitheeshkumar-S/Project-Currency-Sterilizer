#include <LiquidCrystal.h>

//input pin configuration:
const int Control_switch = 2;
const int Hopper_sensor  = 4;
const int Count_sensor_1 = 6;
    
//output pin configuration:
const int UVC_lamp       = 3;
const int Conveyor_motor = 5;

//LCD pin configuration:
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13; //LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

//Lamp declaration:
int uvc_intensity = 145; //in µW/cm2:
int uvc_dose = 7000; //µWs/cm2 for sterlizing various microorganism: 

//Motor declaration//
int max_speed_motor=30; //rpm:
int true_speed=0;
    
//conveyor declaration:
int dia_pulley=50; //mm
int length_conveyor=1000; //mm
    
//PWM declaration:
int dutycycle=0; //(0-100) if 0% means 0rpm 100% means 30rpm:
int pwm_value=0;
    
//counter declaration:
int count=0;
    
//Other
float pi = 3.14 ;  
int flag = 0 ;

int sterlizing_time()
{
  return(uvc_dose/uvc_intensity);   
}

int rpm_calculation()
{
  //V=(d/2)*(2*PI*N)/60://v is linear speed of currency//V=length of conveyor/sterlizing_time:
  return(length_conveyor/sterlizing_time())/(((dia_pulley/2)*(2*pi))/60); 
}

int pwm_calculation()//it return 0-255:
{
  true_speed=rpm_calculation();
  return ((true_speed/max_speed_motor)*255);
}

int kick_start()
{
  return 255;
}

int ideal_mode()
{
  lcd.setCursor(0,0);
  lcd.print("ideal_mode");
  delay(500);

  while(1)
   {
   if((digitalRead(Hopper_sensor))==HIGH)
     { 
       lcd.clear();
       delay(500);
       run_mode();
     }
    else if(digitalRead(Control_switch)==HIGH&&(digitalRead(Hopper_sensor))==LOW)
    {
      flag=0;
      count=0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("//Shutdown//");
      delay(500);
     }
      digitalWrite(UVC_lamp, LOW);
      analogWrite(Conveyor_motor,0);
    }
}

int run_mode()
{
  lcd.setCursor(0,0);
  lcd.print("run_mode");
  lcd.setCursor(0,1);
  lcd.print("count=");
  //analogWrite(Conveyor_motor,kick_start());
  while((digitalRead(Hopper_sensor))==HIGH)
  {
   if((digitalRead(Control_switch)==HIGH)&&flag==1)
   {
    lcd.clear();
    delay(500);
    pause_mode();
   }
   else
   {
    digitalWrite(UVC_lamp, HIGH);
    analogWrite(Conveyor_motor,pwm_value=125);
    if(digitalRead(Count_sensor_1)==HIGH)
     {
      count=count+1;
      delay(500);
     }
    lcd.setCursor(7,1);
    lcd.print(count);
   }
  }
  lcd.clear();
  delay(100);
  ideal_mode();
}

int pause_mode()
{
  lcd.setCursor(0,0);
  lcd.print("pause_mode");
  delay(500);
  digitalWrite(UVC_lamp, LOW);
  analogWrite(Conveyor_motor,0);
  while(1)
  {
   if((digitalRead(Control_switch)==HIGH)&&flag==1)
   {
    lcd.clear();
    delay(1000);
    return run_mode();
   }
   else
    {
      
    }
  }
}

void setup() 
{
  //Input setup
  pinMode(Control_switch, INPUT_PULLUP );
  pinMode(Hopper_sensor , INPUT);
  pinMode(Count_sensor_1, INPUT);

  //Output setup
  pinMode(UVC_lamp , OUTPUT);
  pinMode(Conveyor_motor,OUTPUT);

  //LCD setup
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() 
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(500);
  pwm_value=pwm_calculation();
  while(1)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Off_mode");
   delay(1000);
   if(digitalRead(Control_switch)==HIGH)
    {
     flag=1;
    }
   while(flag==1)
    {
    delay(500);
    lcd.clear();
    ideal_mode();
    }
  }
}
