
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <math.h>
#include "ifttt.h"

#define IR_CONTROL 	0
#define RED_LED		5
#define BLUE_LED	1
#define BUTTON		2
#define BUZZER		4

enum State {/*build a new object enum*/
	ALARM_OFF, ALARM_ARMED, ALARM_ARMING, ALARM_TRIGGERED, ALARM_SOUNDING
};

static enum State alarmState;

void setup() {
	alarmState = ALARM_OFF;
	wiringPiSetup () ;
	pinMode(IR_CONTROL, INPUT);
	pullUpDnControl(IR_CONTROL, PUD_UP);
	pinMode(BLUE_LED, OUTPUT); /*Blue LED*/
	pinMode(RED_LED, OUTPUT); /*Red LED*/
	pinMode(BUTTON, INPUT);
	pullUpDnControl(BUTTON, PUD_UP);
	pinMode(BUZZER, OUTPUT);
}

void alarmOff() {
	printf("off...\n");
	delay(2000);
	digitalWrite(BLUE_LED, HIGH);
	digitalWrite(RED_LED, LOW);
	digitalWrite(BUZZER, LOW);
	if (digitalRead(BUTTON) == 0) {
		printf("arming...prepareopen....\n");	
		alarmState = ALARM_ARMING;
	}
}

void alarmArming() {
	int i = 0;
	while (i < 10) {/*10s*/
		digitalWrite(BLUE_LED, HIGH);
		delay(400);
		digitalWrite(BLUE_LED, LOW);
		delay(400);/*half second*/
		i++;
	}
	digitalWrite(RED_LED, HIGH);
	alarmState = ALARM_ARMED;
}

void alarmArmed() {
	printf("armed....start......\n");
	delay(50);
	if (digitalRead(BUTTON) == 0) {
		alarmState = ALARM_OFF;
	} else if (digitalRead(IR_CONTROL) == 1) {
		alarmState = ALARM_TRIGGERED;
	}
}

void alarmTriggered() {
	printf("triggered open.........\n");
	double previous = (double) time(NULL);
	double finished = (double) time(NULL) + 10;
	double current = (double) time(NULL);
	double interval =1;/**/
	int ledsOn = 1;    
	while(current <= finished) {
		if (digitalRead(BUTTON) == 0) {
			alarmState = ALARM_OFF;
			break;
		}
		if (current - previous > interval) {
			if (ledsOn == 1) {
				digitalWrite(RED_LED, HIGH);
				digitalWrite(BLUE_LED, HIGH);
				ledsOn = 0;
			} else {
				digitalWrite(RED_LED, LOW);
				digitalWrite(BLUE_LED, LOW);
				ledsOn = 1;
			}
			previous = current;
		}
		current = (double) time(NULL);
	}
	alarmState = ALARM_SOUNDING;
}

void alarmSounding() {
	printf("sounding......boom\n");
	ifttt("https://maker.ifttt.com/trigger/112/with/key/bxK2Vz1Sd9_Qiq2mF8RqrQ","1","1","Alarm triggered.");
	double started = (double) time(NULL);
	double current = (double) time(NULL);
	double interval = 1;
	int on = 0;
	while (digitalRead(BUTTON) == 1) {
		current = time(NULL);
		if (current - started >= interval) {
			if (on == 1) {
				digitalWrite(RED_LED, HIGH);
				digitalWrite(BLUE_LED, HIGH);
				digitalWrite(BUZZER, HIGH);
				on = 0;
			} else {
				digitalWrite(RED_LED, LOW);
				digitalWrite(BLUE_LED, LOW);
				digitalWrite(BUZZER, LOW);
				on = 1;
			}
			started = current;
		}
	}
	alarmState = ALARM_OFF;
}

int main(int argc, char *argv[]) {
  printf("x\n");
  setup();
	  
  while(1) {
	  if (alarmState == ALARM_OFF) {
		  alarmOff();
	  } else if (alarmState == ALARM_ARMING) {
		  alarmArming();
	  } else if (alarmState == ALARM_ARMED) {
		  alarmArmed();
	  } else if (alarmState == ALARM_TRIGGERED) {
		  alarmTriggered();
	  } else if (alarmState == ALARM_SOUNDING) {
		  alarmSounding();
	  }	  
  }
  return 0 ;
}
