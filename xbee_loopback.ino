
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>

// Commands to interface with the LCD:
#include <LiquidCrystal.h>

SoftwareSerial XBee(2, 3); // RX, TX


/*================
 * LCD CONNECTIONS:
 *   1 to GND
 *   2 to 5V
 *   3 to the center pin on the potentiometer
 *   4 to Arduino digital pin 12
 *   5 to GND
 *   6 to Arduino digital pin 11
 *   7 (no connection)
 *   8 (no connection)
 *   9 (no connection)
 *   10 (no connection)
 *   11 to Arduino  digital pin 5
 *   12 to Arduino  digital pin 4
 *   13 to Arduino  digital pin 3
 *   14 to Arduino  digital pin 2
 *   15 to 5V
 *   16 to GND
 *====================*/
#define LCD_D7         4 
#define LCD_D6         5
#define LCD_D5         6
#define LCD_D4         7
#define LCD_ENABLE     8
#define LCD_REG_SEL    9

LiquidCrystal lcd(LCD_REG_SEL, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/******** 
 *  Use these pins to activate the tests below by 
 *  pulling them to ground.  Note all are active low.
 *  Also note that the "Random Data" test is orthoganal to the "Empty Ack" 
 *  and "Wrong Data" pin...it can go in parallel.
 *  If neither "Empty Ack" or "Wrong Ack" is on, we'll do a "correct ack"
 *  (sending 'A', then the received data).
 */
// Acks just have "A".  No data.
#define EMPTY_ACK_PIN   13

// Acks use wrong data...send a '0'
#define WRONG_ACK_PIN   12

// Send random data intermittently
#define RANDOM_DATA_PIN 11

void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);

  pinMode(EMPTY_ACK_PIN, INPUT_PULLUP);
  pinMode(WRONG_ACK_PIN, INPUT_PULLUP);
  pinMode(RANDOM_DATA_PIN, INPUT_PULLUP);

  randomSeed(analogRead(0));
  
  Serial.begin(9600);

  Serial.println("XBee Loopback initialized");

  lcd.begin(16,2);  // 2 lines of 16 characters.
  lcd.clear();
  lcd.print("Init...");
}

int random_count = 0;

// 1 in RANDOM_FREQ calls will generate data.  Set to 1 to always generate. 
// Empirically, 10000 gives about one a second.
#define RANDOM_FREQ 10000
void random_data_test(void)
{
  int rand;
  char rand_char;

  rand = random(1, RANDOM_FREQ);
  if (rand == 1)
  {
    Serial.print("Generate Random Char: ");
    rand_char = random(1, 128);
    Serial.print(rand_char);
    Serial.print(" count = ");
    Serial.println(random_count++);
    
    XBee.print(rand_char);
  }
}

void loop()
{
  char loopback_char;
  
  if (!digitalRead(RANDOM_DATA_PIN))
  {
    random_data_test();

    lcd.setCursor(0,0);
    lcd.print("Random Test ON ");    
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Random Test OFF");
  }
  
  if (XBee.available())
  { 
    loopback_char = XBee.read();

    if (!digitalRead(EMPTY_ACK_PIN))
    {
      XBee.write('A');
      Serial.println("Empty ack...");

      lcd.setCursor(0,1);
      lcd.print("Empty Ack ");
    }
    else if (!digitalRead(WRONG_ACK_PIN))
    {
      XBee.write('A');
      XBee.write('0');
      Serial.println("Wrong Ack ");
      
      lcd.setCursor(0,1);
      lcd.print("Wrong Acks");
    }
    else
    {
       // Correct loopback operation
       XBee.write('A');
       XBee.write(loopback_char);
    
       Serial.print("Received ");
       Serial.print(loopback_char);
       Serial.print(" ---> Send A");
       Serial.println(loopback_char);

       lcd.setCursor(0,1);
       lcd.print("loopback ");
       lcd.print(loopback_char);
    }
  }

}
