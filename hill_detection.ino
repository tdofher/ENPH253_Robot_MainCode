/*
 * Checks the state of the Hillpin buttons, and compares them to the old state of the buttons. Adjusts the 
 * global motor speed accordingly (void return values, adjusting mutable types).
 *
 */


/*
 *  I think this function might work better if it is a void function that adjusts global variables so that it fits 
 *  on line 134 of "main_code" (look there for how the structure is gonna be like, which I think works nicely). 
 *  This means that the "num" variable should probably be in the form of some global
 *  variable defined in "main_code", so you can keep track of the previous state of the switches. 
 *  Also keep in mind that when you make this into global form - that you are not repeatedly adding speedAdd 
 *  if the switch has already been pressed and its been added already.
 *  
 *  Also I added a new variable, set HILL_DONE = 1 after you've got up the hill!
 */


void hillDetector() { 
    
   if (digitalRead(HILLpin_front) && HILL_DETECTED == 0) {
      Speed += speedAdd;
      HILL_DETECTED = 1;
   } 
 
   if(digitalRead(HILLpin_back) && HILL_DETECTED == 1) {
      Speed -= speedAdd;
      HILL_DONE = 1;
     } 
}
