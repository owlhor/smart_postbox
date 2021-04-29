#include <TridentTD_LineNotify.h>
#define SSID        "____________________________"
#define PASSWORD    "____________________________"
#define LINE_TOKEN  "________your LineNotify token____________"  


int led1 = D0; // กำหนดขาใช้งาน
int buttonPin = D1;
int resetpin = D8;
int buttonState = 0;
int count = 0,delta,resetter;

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
  Serial.println(buttonState);
  Serial.println(resetter);
  

  if (delta == -1){
    count++;
    Serial.println("count = ");
    Serial.println(count, DEC);
    String ib = "inbox = ";
    String inbox = ib + count;
    LINE.notify(inbox);
  }
  if (resetter == 1){
    count=0;
    Serial.println("postbox is resetted");
    LINE.notify("postbox is reset to 0.");
    }
  
  delay(500);
}
