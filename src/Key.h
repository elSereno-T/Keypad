/*
||
|| @file Key.h
|| @version 1.0
|| @author Mark Stanley
|| @contact mstanley@technologist.com
||
|| @description
|| | Key class provides an abstract definition of a key or button
|| | and was initially designed to be used in conjunction with a
|| | state-machine.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef Keypadlib_KEY_H_
#define Keypadlib_KEY_H_

#include <Arduino.h>

#define OPEN LOW
#define CLOSED HIGH

typedef unsigned int uint;
typedef enum{ IDLE, PRESSED, HOLD, REPEATING, RELEASED } KeyState;
const String KeyStateString[] = {"IDLE", "PRESSED", "HOLD", "REPEATING", "RELEASED"};
typedef enum{ REPEAT, ALTREPEAT, ALT} KeyType;


typedef struct{ uint8_t row; uint8_t col; } GPIO_struct ;

const char NO_KEY = '\0';

class Key {
public:
	// members
	char mainChar; // primary character
	char altChar; // secondary character
	char typeChar;

	GPIO_struct GPIO;
	// int kcode; // key number (1D)
	// int krow; // key row
	// int kcol; // key colum
	unsigned long start; // time of start of press
	unsigned long send; // time of last send
	uint16_t wait; // current wait period
	KeyState keyState; // key state
	bool buttonState; // true if pushed
	bool lastReading; // true if it was pushed last time
	bool stateChanged; // true if state has changed


	// methods
	// Key();
	// Key(char userKeyChar);
	// Key(uint16_t userKeyChar);
	// void key_update(char userKeyChar, KeyState userState, boolean userStatus);
	char update(bool pressed);
	void changeTo(KeyState newState);
	void read();
	void init(char userMainChar, char userAltChar, uint8_t row_GPIO, uint8_t col_GPIO,  String no_repeat, uint16_t DEBOUNCE_MS, uint16_t HOLD_TIME, uint16_t REPEAT_DELAY, uint16_t REPEAT_ACCELERATION, uint16_t REPEAT_MAX_RATE 
	// int start,
	// int send,
	// int wait,
	// KeyState kstate,
	// KeyType ktype,
	// bool pushed,
	// bool lastPushed,
	// bool stateChanged
);
	bool veryLongPress(int time);

private:


	KeyType ktype; // key type

	uint16_t debounce;
	uint16_t hold;
	uint16_t delay;
	uint16_t acceleration;
	uint16_t max_rate;

};

#endif

/*
|| @changelog
|| | 1.0 2012-06-04 - Mark Stanley : Initial Release
|| #
*/
