
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
#include <Wire.h>   // use touchpot to control tests.

SoftwareSerial XBee(2, 3); // RX, TX

/* Touchpot Register definitions
/* see http://danjuliodesigns.com/products/touch_pot/assets/touch_pot_sf_1_4.pdf */
#define TOUCHPOT_VERSION       0   // Read only
#define TOUCHPOT_CUR_POT_VALUE 1   // RW, Current poteniometer value
#define TOUCHPOT_STATUS        2   // Read only, Device Status
#define TOUCHPOT_CONTROL       3   // RW, device configuration
#define TOUCHPOT_USER_LED      4   // RW, user set led value.

int i2cAddr = 8; // Direct access at i2cAddr, indirect registers at i2cAddr+1

// 74595 lines
#define DATA_PIN  A0
#define CLK_PIN   A1
#define LATCH_PIN A2

// We have 8 output LEDs in our current incarnation.
#define OUTPUT_LINES 8


void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Wire.begin();

  randomSeed(analogRead(0));
  
  Serial.begin(9600);

  Serial.println("Jeep ack Loopback initialized");

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

  if (XBee.available())
  { 
    loopback_char = XBee.read();

    // Correct loopback operation
    Serial.print("Looping back ");
    Serial.print(loopback_char);
  }

}
