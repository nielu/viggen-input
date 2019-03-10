/*
 Name:		Viggen_Panel.ino
 Created:	3/8/2019 5:13:43 PM
 Author:	nielu
*/
#define DCSBIOS_IRQ_SERIAL
//#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <internal/PollingInput.h>

#define KEY_0 0
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 4
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8
#define KEY_9 9
#define KEY_LS_SKU 10
#define KEY_L_MAL 11
#define KEY_UNKNOWN 12

#define PIN_DATA_IN_OUT 2
#define PIN_KEYPAD A0

#define DEBUG_ON 0

const char* const KEY_NAME[]{
		  "0","1","2","3","4","5","6",
		  "7","8","9","LS_SKU","L_MAL",
		  "UNKNOWN"
};

const char* KEY_FUNCTION[]{
	"DATAPANEL_KEY_0", "DATAPANEL_KEY_1", "DATAPANEL_KEY_2", "DATAPANEL_KEY_3",
	"DATAPANEL_KEY_4", "DATAPANEL_KEY_5", "DATAPANEL_KEY_6", "DATAPANEL_KEY_7",
	"DATAPANEL_KEY_8", "DATAPANEL_KEY_9", "NAV_SELECT_BTN_LS", "NAV_SELECT_BTN_L_MAL",
	"UNKNOWN"
};

#if DEBUG_ON

uint8_t lastKey = KEY_UNKNOWN;
uint8_t getKey(float val) {
	if (val < 30)
		return KEY_UNKNOWN;
	if (val > 760)
		return KEY_7;
	if (val > 420)
		return KEY_8;
	if (val > 250)
		return KEY_9;
	if (val > 160)
		return KEY_4;
	if (val > 137)
		return KEY_5;
	if (val > 111)
		return KEY_6;
	if (val > 90)
		return KEY_1;
	if (val > 82)
		return KEY_2;
	if (val > 71)
		return KEY_3;
	if (val > 62)
		return KEY_0;
	if (val > 58)
		return KEY_LS_SKU;
	return KEY_L_MAL;
}

void setup() {
	Serial.begin(115200);
	for (uint8_t i = 0; i < 13; i++) {
		Serial.print("i: ");
		Serial.print(i);
		Serial.print(" name: ");
		Serial.print(KEY_NAME[i]);
		Serial.print(" func: ");
		Serial.println(KEY_FUNCTION[i]);
	}
}

void loop() {
	float a0 = analogRead(A0);
	uint8_t key = getKey(a0);
	if (key != lastKey) {
		lastKey = key;
		Serial.print("KEY: ");
		Serial.print(key);
		Serial.print(" NAME: ");
		Serial.print(KEY_NAME[key]);
		Serial.print(" FUNC: ");
		Serial.print(KEY_FUNCTION[key]);
		Serial.print(" VALUE: ");
		Serial.println(a0);
	}
}
#else 

namespace DcsBios {
	class AnalogInputKeypad : PollingInput {
	private:
		uint8_t lastKey_;
		uint8_t inputPin_;
		const char* const* key_func_;


		uint8_t getKey(float val) {
			if (val < 30)
				return KEY_UNKNOWN;
			if (val > 760)
				return KEY_7;
			if (val > 420)
				return KEY_8;
			if (val > 250)
				return KEY_9;
			if (val > 160)
				return KEY_4;
			if (val > 137)
				return KEY_5;
			if (val > 111)
				return KEY_6;
			if (val > 90)
				return KEY_1;
			if (val > 82)
				return KEY_2;
			if (val > 71)
				return KEY_3;
			if (val > 62)
				return KEY_0;
			if (val > 58)
				return KEY_LS_SKU;
			return KEY_L_MAL;
		}

		void pollInput() {
			uint8_t currentKey = getKey(analogRead(inputPin_));
			if (currentKey != lastKey_ && lastKey_ != KEY_UNKNOWN) {
				tryToSendDcsBiosMessage(KEY_FUNCTION[lastKey_], "0");
			}
			if (currentKey != KEY_UNKNOWN) {
				tryToSendDcsBiosMessage(KEY_FUNCTION[currentKey], "1");
			}
			lastKey_ = currentKey;
		}

	public:
		AnalogInputKeypad(const char* const* key_func , const uint8_t inputPin) : lastKey_(0) {
			key_func_ = key_func;
			inputPin_ = inputPin;
			lastKey_ = getKey(analogRead(inputPin_));
		}
	};
}

DcsBios::Switch2Pos dataInOut("DATA_IN_OUT", PIN_DATA_IN_OUT);
DcsBios::AnalogInputKeypad analogKeypad(KEY_FUNCTION, A0);

void setup() {
	DcsBios::setup();
}

void loop() {
	DcsBios::loop();
}

#endif
