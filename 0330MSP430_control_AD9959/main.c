#include <MSP430G2553.h>
#include <AD9959.h>
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	 //-----DCOÉèÎª12MHz----
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;

	Init_AD9959();
	return 0;
}
