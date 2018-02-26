#define TL 3
#define TR 5
#define BL 6
#define BR 9

#define PC_TL 0
#define PC_TR 1
#define PC_BL 2
#define PC_BR 3

// Despite the name, this struct is input data from the PC, with its name copypastad from the app
struct ArduinoOutputData {
  uint8_t NumFlashes[2]; // Number of flashes for each side in a flash sequence
  uint8_t LedBrightnesses[4]; // Use the PC_ constants for these
  uint8_t NullTerminator;
};

ArduinoOutputData inputData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("Let's do this!");

  // Initialise ports
  pinMode(TR, OUTPUT);
  pinMode(TL, OUTPUT);
  pinMode(BR, OUTPUT);
  pinMode(BL, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  while (Serial.available() > 0) {
    Serial.read();
  }
}

void loop() {
  // Read light data from app
  while (Serial.available() >= sizeof (inputData)) {
    // Read into the buffer
    Serial.readBytesUntil(127, (char*)&inputData, sizeof (inputData));
  }
  
  // Send potentiometer data to app
  Serial.println(String(analogRead(A0)) + "-" + String(analogRead(A1)));

  // Update the lights
  // Flash byte
  int numLeftFlashes = min(inputData.NumFlashes[0], 10);
  int numRightFlashes = min(inputData.NumFlashes[1], 10);

//Serial.println("Data: " + String(inDataBuffer[0]) + " " + String(inDataBuffer[1]) + " " + String(inDataBuffer[2]) + " " + String(inDataBuffer[3]) + " " + String(inDataBuffer[4]) + " " + String(inDataBuffer[5]) + " ");

  if (numLeftFlashes > 0 || numRightFlashes > 0) {
    // Turn all lights off first
    digitalWrite(TL, false);
    digitalWrite(TR, false);
    digitalWrite(BL, false);
    digitalWrite(BR, false);
    
    // Flash all the lights to the count requested by the game
    for (int i = 0; i < numLeftFlashes || i < numRightFlashes; ++i) {
      if (i < numLeftFlashes)
      {
        digitalWrite(TL, true);
        digitalWrite(BL, true);
      }
      if (i < numRightFlashes)
      {
        digitalWrite(TR, true);
        digitalWrite(BR, true);
      }
      delay(50);
      if (i < numLeftFlashes)
      {
        digitalWrite(TL, false);
        digitalWrite(BL, false);
      }
      if (i < numRightFlashes)
      {
        digitalWrite(TR, false);
        digitalWrite(BR, false);
      }
      delay(50);
    }
  }

  // Light bytes (yum)
  analogWrite(TL, inputData.LedBrightnesses[PC_TL]);
  analogWrite(TR, inputData.LedBrightnesses[PC_TR]);
  analogWrite(BL, inputData.LedBrightnesses[PC_BL]);
  analogWrite(BR, inputData.LedBrightnesses[PC_BR]);

  delay(5); // Rapid update time to reduce input lag
}
