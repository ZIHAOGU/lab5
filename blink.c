#include <wiringPi.h>
#include <stdio.h>
int main (int argc, char *argv[])
{
  int i;
	wiringPiSetup () ;
	pinMode (1, OUTPUT) ;
	pinMode(3, INPUT);
	pinMode (5, OUTPUT) ;
	pinMode (4, OUTPUT) ;
	while(1) {
		printf("it fucking works\n");
		digitalWrite(5, HIGH);
		delay(500);
		digitalWrite(5, LOW);
		delay(500);	
	
}
/*  for (i=0;i<10;i++) {
    digitalWrite (1, HIGH) ; delay (500) ;
    digitalWrite (1,  LOW) ; delay (500) ;
  }
  for (i=0;i<10;i++) {
    digitalWrite (2, HIGH) ; delay (500) ;
    digitalWrite (2,  LOW) ; delay (500) ;
  }
  for (i=0;i<10;i++) {
    digitalWrite (4, HIGH) ; delay (500) ;
    digitalWrite (4,  LOW) ; delay (500) ;
  }*/
  return 0 ;
}
