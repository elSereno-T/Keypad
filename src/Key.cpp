/*
|| @file Key.cpp
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
#include <Key.h>

void Key::changeTo(KeyState newState){
	keyState = newState;
	stateChanged = true;
	Serial.println(String(mainChar) + " " + KeyStateString[newState]);
}
char Key::read(bool set_row){

	digitalWrite(GPIO.row, LOW);
	delayMicroseconds(30);
	char out = read();
	digitalWrite(GPIO.row, HIGH);
	return out;
}
char Key::read(){
	return update(digitalRead(GPIO.col));
}
char Key::update(int read_value){
	bool reading = read_value == CLOSE;
	unsigned long now = millis();
	if (reading != lastReading) start = now;
	lastReading = reading;
	stateChanged=false;
	if ((now - start) > debounce) {
		if (reading != buttonState) {
		buttonState = reading;  // State is stable — accept it
		stateChanged=true;
		}
	}
	switch (keyState){
		case IDLE:
			if (buttonState){
				send = now;
				if (ktype == REPEAT) 	changeTo(REPEATING);
				else					changeTo(PRESSED);
			}
			break;
		case PRESSED:
			if (stateChanged) {
				changeTo(RELEASED);					
				return mainChar;
			}
			if ((now - start) > hold) {
				if (ktype == ALTREPEAT)	changeTo(REPEATING);
				else {					changeTo(HOLD);
					return altChar;
				}
			} 
			break;
		case HOLD:
			if (stateChanged) changeTo(RELEASED);
			break;
		case RELEASED:
			changeTo(IDLE);
			wait = delay;
			break;
		case REPEATING:
			if (stateChanged){
				changeTo(RELEASED);
				return altChar;
			}
			if ((now-send) >= wait){
				send += wait;
				wait = uint16_t(max(int(wait-acceleration), int(max_rate)));
				return altChar;
			}
			break;
 	}
	return char(0);
}
bool Key::veryLongPress(int time){
	return (buttonState && (millis()-start) > time);
}
void Key::init(char userMainChar, char userAltChar, uint8_t row_GPIO, uint8_t col_GPIO, String no_repeat, uint16_t DEBOUNCE_MS, uint16_t HOLD_TIME, uint16_t REPEAT_DELAY, uint16_t REPEAT_ACCELERATION, uint16_t REPEAT_MAX_RATE ){
	mainChar = userMainChar;
	altChar = userAltChar;
	
	GPIO.row = row_GPIO;
	GPIO.col = col_GPIO;
	// krow = row;
	// kcol = col;
	// kcode = row * rows + col;

	if (mainChar == altChar) ktype = REPEAT;
	else if (no_repeat.indexOf(altChar) > -1) ktype = ALT;
	else ktype = ALTREPEAT;

	keyState = IDLE;
	buttonState = false;
	lastReading = false;
	stateChanged = false;
	debounce = DEBOUNCE_MS;
	hold = HOLD_TIME;
	delay = REPEAT_DELAY;
	acceleration = REPEAT_ACCELERATION;
	start = 0;
	send = 0;
	wait = delay;
}



/*
|| @changelog
|| | 1.0 2012-06-04 - Mark Stanley : Initial Release
|| #
*/
