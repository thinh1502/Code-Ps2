#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT     13
#define PS2_CMD     11 
#define PS2_SEL     10
#define PS2_CLK     12
#define IN1 6
#define IN2 5
#define IN3 3
#define IN4 4


/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;
int j;
void setup(){
  Serial.begin(57600);
  
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  Serial.println("ahihi");
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
	if (pressures)
	  Serial.println("true ");
	else
	  Serial.println("false");
	Serial.print("rumble = ");
	if (rumble)
	  Serial.println("true)");
	else
	  Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
	case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}
void motor_1_Dung() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
void motor_2_Dung() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void motor_1_Lui()  { 
  analogWrite(IN1, 200);
  digitalWrite(IN2, LOW);
}
void motor_1_Tien() {
   digitalWrite(IN1, LOW);
  analogWrite(IN2, 200);
}
void motor_2_Lui()  { 
  analogWrite(IN3, 200);
  digitalWrite(IN4, LOW);
}
void motor_2_Tien() {
   digitalWrite(IN3, LOW);
  analogWrite(IN4, 200);
}


void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1) //skip loop if no controller found
    return; 
  
  if(type == 2){ //Guitar Hero Controller
    ps2x.read_gamepad();          //read controller 
   
  /* if(ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if(ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if(ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if(ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if(ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed");
 
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held"); */
  }  else  { //Phần di chuyển
    ps2x.read_gamepad(); //read controller and set large motor to spin at 'vibrate' speed
    
        

    if(ps2x.Button(PSB_PAD_DOWN)||ps2x.Analog(PSS_LY)==255) {      //will be TRUE as long as button is pressed
     Serial.println("lùi");                            
                      motor_1_Lui(); motor_2_Lui();    
                      delay(50);   
                      motor_1_Dung();  motor_2_Dung(); 
    }
    if(ps2x.Button(PSB_PAD_RIGHT)||ps2x.Analog(PSS_LX)==0){
        Serial.println("Quẹo phải");     motor_1_Tien();      delay(50);   motor_1_Dung();  }  
    
    if(ps2x.Button(PSB_PAD_LEFT)||ps2x.Analog(PSS_LX)==255){
      Serial.println("Quẹo trái");     motor_2_Tien();      delay(50);   motor_2_Dung();     }
      
    if(ps2x.Button(PSB_PAD_UP)||ps2x.Analog(PSS_LY)==0){
         
                      Serial.println("tiến");                            
                      motor_2_Tien(); motor_1_Tien();     
                      delay(50);   
                      motor_2_Dung(); motor_1_Dung();} 
  
   //  vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if(ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if(ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");        
    }

  /*  if(ps2x.ButtonPressed(PSB_CIRCLE)) {   
      j++;Serial.print(j);delay(200);          
     while (j%2==1) {
      
      if (digitalRead(7)==0){Serial.print("Trai");motor_2_Tien_3(); motor_1_Lui_2();    delay(50);   motor_2_Dung();motor_1_Dung();   }
      if (digitalRead(8)==0){
        Serial.println("tiến");                            
                      motor_1_Tien_2(); motor_2_Tien_2();    
                      delay(20);   
                      motor_1_Dung();  motor_2_Dung(); }
      if (digitalRead(9)==0){Serial.print("phai");motor_1_Tien_3(); motor_2_Lui_2();     delay(200);   motor_1_Dung();motor_2_Dung();}
      
      if ((digitalRead(7)==1)&&(digitalRead(8)==1)&&(digitalRead(9)==1)) {
       Serial.println("tiến");                            
                      motor_1_Tien_2(); motor_2_Tien_2();    
                      delay(20);   
                      motor_1_Dung();  motor_2_Dung(); }
       ps2x.read_gamepad();   
      if(ps2x.ButtonPressed(PSB_CIRCLE)) {   j++;Serial.print(j);}
      
     }
    }*/
  /*  if(ps2x.Button(PSB_CROSS)||ps2x.Analog(PSS_RY)==255)  {             //will be TRUE if button was JUST pressed OR released
  //  if(j%2==1){
      Serial.println("lùi phải");     motor_2_Lui();       delay(50);  motor_2_Dung();   } 
    else
      {  Serial.println("lùi");                            
                      motor_1_Lui(); motor_2_Lui();    
                      delay(50);   
                      motor_1_Dung();  motor_2_Dung();   }}
    if((ps2x.Button(PSB_SQUARE)) ||(ps2x.Analog(PSS_RY)==0))        {     //will be TRUE if button was JUST released
   // if(j%2==1){
      Serial.println("tiến phải");     motor_2_Tien();      delay(50);   motor_2_Dung();   }
  /*  else
      {  Serial.println("tiến");                            
                      motor_2_Tien(); motor_1_Tien();     
                      delay(50);   
                      motor_2_Dung(); motor_1_Dung();   } */
      if(ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
    if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
      Serial.println("Square just released");     


    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC); 
    }     
  }
  delay(20);
}
