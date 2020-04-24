/*
  SerialOutBTM0.ino
  Serial output via Bluetooth for Arduino M0
  (c) Claus Kuehnel 2018-12-16 info@ckuehnel.ch

  Connect
  Arduino®   HC-10
  ----------------
  5 V        VCC
  GND        GND
  D10        Rx
  D12        Tx
*/

// Serial2 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
#define PIN_SERIAL2_RX       (34ul)               // Pin description number for PIO_SERCOM on D12
#define PIN_SERIAL2_TX       (36ul)               // Pin description number for PIO_SERCOM on D10
#define PAD_SERIAL2_TX       (UART_TX_PAD_2)      // SERCOM pad 2
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_3)    // SERCOM pad 3

// Instantiate the Serial2 class
Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void SERCOM1_Handler()    // Interrupt handler for SERCOM1
{
  Serial2.IrqHandler();
}

// define your board here
//#define BOARD "Arduino Leonardo"
#define BOARD "Arduino M0"

#define Serial SerialUSB // Arduino M0 native port

void setup() 
{
  Serial.begin(115200);
  while(!Serial);       // for native port
  // delay(2000);
  Serial.print(BOARD);
  Serial.println(" starts BT communication...");
  
  Serial2.begin(9600);          
  Serial2.print("This is ");
  Serial2.println(BOARD);
}

void loop() 
{
  Serial.print("*");
  Serial2.print("*");
  delay(1000);
}
