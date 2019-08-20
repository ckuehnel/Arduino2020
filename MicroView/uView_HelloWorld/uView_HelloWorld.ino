#include <MicroView.h>

#define BOARD "MicroView"

void setup() {
	uView.begin();				      // start MicroView
	uView.clear(PAGE);			    // clear page
	uView.print("HelloWorld");	// display HelloWorld
  uView.print("This is\n");
  uView.println(BOARD);
  uView.print("Clock\n ");
  uView.print(F_CPU/1000000);      // Oscillator frequency
  uView.print(" MHz");
	uView.display();
}

void loop () {}
