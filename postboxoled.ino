#include <TridentTD_LineNotify.h>
#define SSID        "IoT Contest AirNet A"
#define PASSWORD    "happyiotcontest"
#define LINE_TOKEN  "________your LineNotify token____________"  
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SH1106Wire.h", legacy include: `#include "SH1106.h"`
//#include "images.h"


int led1 = D0; // กำหนดขาใช้งาน
int buttonPin = D1;
int resetpin = D8;
int buttonState = 0;
int count = 0,delta,resetter;

// displaydefine
SH1106Wire display(0x3c, D3, D5);
#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

void setup()
{ // pin
  pinMode(led1, OUTPUT); 
  pinMode(buttonPin, INPUT);
  digitalWrite(led1, LOW);
  Serial.begin(9600);

  // line --------------------------------------------------
  Serial.println();
  Serial.println(LINE.getVersion());
  
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
}

int myfunc(int x){
  static int buf1,buf,delta;
  
  //buf2 = buf1;
  buf1 = buf;
  buf = x;
  delta = x - buf1;
  return delta;
}

void loop()
{
  buttonState = digitalRead(buttonPin); // อ่านค่าสถานะขาD0
  resetter = digitalRead(resetpin);
  delta = myfunc(buttonState);
  Serial.println(buttonState, resetter);
  //Serial.println(resetter);

  display.clear();
  //demos[demoMode]();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Post Counter");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 20, "inbox Post =");
  display.setFont(ArialMT_Plain_24);
  display.drawString(64, 15, String(count));
  display.display();

  if (delta == -1){
    count++;
    Serial.println("count = ");
    Serial.println(count, DEC);
    String ib = "inbox post = ";
    String inbox = ib + count;
    LINE.notify(inbox);
    
    /*display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(5, 15, inbox);
    display.display();
    */
  }
  if (resetter == 1){
    count=0;
    Serial.println("postbox is resetted");
    LINE.notify("postbox is reset to 0.");
    
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(25, 45, "Reset");
    display.display();
    }
  
  delay(250);
}
