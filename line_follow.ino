

void line_follow() {
  
      /***************************************
       * LINE FOLLOWING CODE NOW
       ***************************************/
  
      int left = analogRead(leftSensor);
      int right = analogRead(rightSensor);
  
      if ( (left>threshold) && (right>threshold) ) error = 0;
      if ( (left>threshold) && (right<threshold) ) error = -1;
      if ( (left<threshold) && (right>threshold) ) error = +1;
      if ( (left<threshold) && (right<threshold) ) {
        
        if ( lastError>0 ) error = 5;
        if ( lastError<0 ) error=-5;
        
      }
      
      if (!(error==lastError)) {
        
        recError=lastError;
        q = m;
        m = 1;
        
      }
  
      P = kP * error;
      
      D = (int)((float)kD * (float)(error-recError) / (float)(q+m));
  
      I = kI * error + I;
        if (I>maxI) I = maxI;
        if (I<-maxI) I = -maxI;
      
      con = P + D + I;
  
      if ( c==30 ) {
        
        c = 0;
        
      }
      
      c++;
      m++;
      motor.speed(leftMotor,(Speed-con));
      motor.speed(rightMotor,(Speed+con));
  
      if ( i > 300 ) {
        
        LCD.clear();
        LCD.print(error);
        LCD.setCursor(0,1);
        LCD.print("FLAG: "); 
        LCD.print(FLAG);
        i = 0;
        
      }
      lastError = error;
      i++;
    }
    i = 0;
    //delay(1000);
     
  }


