#ifndef CD4021BE_h
#define CD4021BE_h

#include "Arduino.h"
#include "CObject.h"

class CCD4021BE : public CObject
{
	public:
		CCD4021BE(int latch, int data, int clock, void (*pCbFunctionPressed)(CCD4021BE*)); 
		virtual void setup(); 	
		virtual void loop();  
		boolean getInputState(int pos);		
				
	protected:
		void (*pCallbackTestBtns)(CCD4021BE*);
	
		int _latchPin, _dataPin, _clockPin;
		byte switchVar1;
		char note2sing[8] = {
			'C', 'd', 'e', 'f', 'g', 'a', 'b', 'c'
		};
		boolean _inputStates[8] = {
			false,
			false,
			false,
			false,
			false,
			false,
			false,
			false			
		};
		
		byte shiftIn();
};
#endif