#include <LiquidCrystal.h>
//Dino positions
#define run_1 0//run position 1
#define run_2 1//run position 2
#define jump_high 2//jump high
#define jump_half_head 3//jump half upper
#define jump_half_body 4//jump half lower

#define upperObject1 1
#define upperObject2 2
#define lowerObject1 3
#define lowerObject2 4
#define empty 5

//initialize lib with interface pin #
LiquidCrystal lcd (11,9,6,5,4,3);
static bool buttonPushed = false;
static byte currentPos;
static byte currentPosUp;
static byte currentPosDown;
static bool playing = false;
const int buttonPin = 2;
int buttonState = 0;
static byte upperBG[17];
static byte lowerBG[17];

void graphics(){
byte dino[40] = {
  //1-Run 1
 0b11111,
 0b10111,
 0b11000,
 0b11101,
 0b11110,
 0b11100,
 0b10100,
 0b10010,
 //2-Run 2
 0b11111,
 0b10111,
 0b11000,
 0b11100,
 0b11110,
 0b11100,
 0b01100,
 0b01110,
 //3-Jump High 
 0b11111,
 0b10111,
 0b11000,
 0b11101,
 0b11110,
 0b11100,
 0b11011,
 0b00000,
 //4-Jump Half (Head)
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b00000,
 0b11111,
 0b10111,
 //5-Jump Half (Body)
 0b11000,
 0b11101,
 0b11110,
 0b11100,
 0b11011,
 0b00000,
 0b00000,
 0b00000,
};
int g;
//draw dino positions
  for (g = 0; g < 5; g++) {
    lcd.createChar(g, &dino[g*8]);
  }
}
void drawDino0(){
  //Starting screen
  lcd.setCursor (0,0);
  lcd.print("Press to Start!");
  lcd.setCursor (0,1);
  currentPos = run_1;
  lcd.write(byte(currentPos));
}
void drawDinoWalk(){
  lcd.setCursor(0,1);
  currentPos = run_2;
  lcd.write(byte(currentPos));
  delay(250);
  lcd.setCursor(0,1);
  currentPos = run_1;
  lcd.write(byte(currentPos));
  delay(250);
}
void drawDinoJump(){
  //half up + down
  lcd.setCursor (0,0);
  currentPosUp = jump_half_head;
  lcd.write(byte(currentPosUp));
  lcd.setCursor (0,1);
  currentPosDown = jump_half_body;
  lcd.write(byte(currentPosDown));
  
  delay(200);
  
  //up
  lcd.clear();
  lcd.setCursor (0,0);
  currentPosUp = jump_high;
  lcd.write(byte(currentPosUp));
  
  
  delay(250);
  
  //down
  lcd.setCursor (0,0);
  currentPosUp = jump_half_head;
  lcd.write(byte(currentPosUp));
  lcd.setCursor (0,1);
  currentPosDown = jump_half_body;
  lcd.write(byte(currentPosDown));
  delay(200);
  lcd.clear();
}
void buttonPush() {
  buttonPushed = true;
}

void object(){
  byte object[40] = {
    //1-Upper1
    0b11111,
    0b11111,
    0b11111,
    0b11110,
    0b11110,
    0b11110,
    0b00000,
    0b00000,
    //2-Upper2
    0b00000,
    0b00000,
    0b10001,
    0b01010,
    0b00100,
    0b00000,
    0b00000,
    0b00000,
    //3-Lower1
    0b00000,
    0b00000,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    //4-Lower2
    0b00000,
    0b00000,
    0b00100,
    0b01110,
    0b01110,
    0b11111,
    0b11111,
    0b11111,
    //5-Empty 
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    };
  
  int o;
//draw objects
  for (o = 0; o < 5; o++) {
    lcd.createChar(o, &object[o*8]);
  }
  for (o = 0; o < 17; o++){
    upperBG [o] = empty;
    lowerBG [o] = empty;
  }
}
void objectRandom (byte upperBG[], byte lowerBG[]){

  int ObjectPos = random(1,8);
  
  //Object Array
  switch (ObjectPos){
  case upperObject1:
    lowerBG[16] = byte(empty);
    //upper and lower too close
    if ((lowerBG[15] == lowerObject1)||(lowerBG[15] == lowerObject2)) upperBG[16] = byte(empty);
    else upperBG[16] = byte(upperObject1);    
   break;
  case upperObject2:
    lowerBG[16] = byte(empty);
    //upper and lower too close
    if ((lowerBG[15] == lowerObject1)||(lowerBG[15] == lowerObject2)) upperBG[16] = byte(empty);
    else upperBG[16] = byte(upperObject2);       
   break;
  case lowerObject1:
    //Max two lower objects in a row or upper and lower too close
    if ((lowerBG[15] == lowerObject1)|| (lowerBG[15] == lowerObject2) || (upperBG[15] == upperObject1)||(upperBG[15] == upperObject2)){
      lowerBG[16] = empty;
    }
    else lowerBG[16] = byte(lowerObject1);
  case lowerObject2:
    upperBG[16] = byte(empty);
    //Max two lower objects in a row or upper and lower too close
    if ((lowerBG[15] == lowerObject1)|| (lowerBG[15] == lowerObject2) || (upperBG[15] == upperObject1)||(upperBG[15] == upperObject2)){
      lowerBG[16] = empty;
    }
    else lowerBG[16] = byte(lowerObject2);
   break;
  case empty:
  case 6: //empty2
  case 7: //empty3
  case 8: //empty4
    upperBG[16] = byte(empty);
    lowerBG[16] = byte(empty);
   break;
  }
}

void drawObject (){
  lcd.setCursor (1,16);
  lcd.write (upperBG[16]);
  lcd.setCursor (2,16);
  lcd.write (lowerBG[16]);
}

void setup() {
  //Readwrite
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  //Contrast
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  //Button
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  //Autoplay
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  
  attachInterrupt(0/*PIN_BUTTON*/, buttonPush, FALLING);
  
  graphics();
  object();
  //set up LCD (columns,rows)
  lcd.begin(16, 2);
}
void loop() {
  if (!playing) {
    drawDino0();
  }
  else {
    drawDinoWalk();
    if(buttonPushed){
      drawDinoJump();
      buttonPushed = false;
    }
  }
  
  if (buttonPushed) {
    graphics();
    playing = true;
    buttonPushed = false;
    lcd.clear();
  }
  
  //Generate object
  while (playing == true){
  drawObject();
  }
    return;
}
