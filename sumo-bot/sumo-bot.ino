#include <ESP32Servo.h>
#include <Bluepad32.h>

// create two servo objects 
Servo right;
Servo left;

// declare servo pins
int rightPin = 32;
int leftPin = 33;
int zeroOffset = 2;

// create gamepad pointer for xbox controller
GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

// nate's gamepad functions
// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == nullptr) {
      Serial.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      GamepadProperties properties = gp->getProperties();
      Serial.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n",
                    gp->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myGamepads[i] = gp;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
        "CALLBACK: Gamepad connected, but could not found empty slot");
  }
}
void onDisconnectedGamepad(GamepadPtr gp) {
  bool foundGamepad = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == gp) {
      Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
      myGamepads[i] = nullptr;
      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
        "CALLBACK: Gamepad disconnected, but not found in myGamepads");
  }
}

void setup() {
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	Serial.begin(115200);
	left.setPeriodHertz(50);      // Standard 50hz servo
	right.setPeriodHertz(50);      // Standard 50hz servo
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t *addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2],
                addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();
  pinMode(rightPin, OUTPUT);
  pinMode(leftPin, OUTPUT);
  left.attach(leftPin);
  right.attach(rightPin);
  //right.write(0);
  //left.write(0);
  //delay(500);
  //right.write(180);
  //left.write(180);
  //delay(500);
}

// Main loop, CPU 1
void loop() {
  // XBOX controller stuff
	// This call fetches all the gamepad info from the NINA (ESP32) module.
  // Just call this function in your main loop.
  // The gamepads pointer (the ones received in the callbacks) gets updated
  // automatically.
  BP32.update();
  // It is safe to always do this before using the gamepad API.
  // This guarantees that the gamepad is valid and connected.
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    GamepadPtr myGamepad = myGamepads[i];

    if (!(myGamepad && myGamepad->isConnected())) continue;
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...
    //forward
    if (myGamepad->y()) {
      right.write(0);
      left.write(180);
      Serial.printf("Y pressed, moving forward.\n");
      delay(500);
      right.write(90+zeroOffset);
      left.write(90+zeroOffset);
    }
    //back
    if (myGamepad->a()) {
      right.write(180);
      left.write(0);
      Serial.printf("A pressed, moving backward.\n");
      delay(500);
      right.write(90+zeroOffset);
      left.write(90+zeroOffset);
    }
    //left
    if (myGamepad->x()) {
      right.write(0);
      left.write(0);
      Serial.printf("X pressed, turning left.\n");
      delay(500);
      right.write(90+zeroOffset);
      left.write(90+zeroOffset);
    }
    //right
    if (myGamepad->b()) {
      right.write(180);
      left.write(180);
      Serial.printf("B pressed, turning right.\n");
      delay(500);
      right.write(90+zeroOffset);
      left.write(90+zeroOffset);
    }
    if (myGamepad->dpad() == 1) {
      zeroOffset = zeroOffset + 1;
      Serial.printf("offset up, %d\n", zeroOffset);
    }
    if (myGamepad->dpad() == 2) {
      zeroOffset = zeroOffset - 1;
      Serial.printf("offset down, %d\n", zeroOffset);
    }
  }
  

	//right.detach();
	//left.detach();

	delay(50);

}