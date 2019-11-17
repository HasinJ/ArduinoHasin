
 const int Rtrig = 2;
 const int Recho = 3;
 const int Ltrig = 4;
 const int Lecho = 5;
 long timeTravel, distance; //distance's a float just in case hands are a little over sweet spot
 int Ldist, Rdist, Rdistold, Ldistold, distToTrigger = 22, maxRange = 50, minRange = 1, speedHalt=300;
 
 

 /* for play/pause
  *  we want to have both hands to stay a certain sweet spot for the sensors to track
  *  however we want the sweet spot to stay a reasonable distance
  */

 /* for next/prev song
  * both hands must satisfy the sweet spots of both the sensors, but at different times
  * try not to get it confused with mistake gestures
  * reasonable time measurement and distance
  */

 /* for continuous next/prev
  *  the distance has to be different from "old" distance but with a little generousity, or a little assurance (both just in case the object/person doesn't actually want to do anything
  *  reasonable time and distance
  *  take into account if the person is 1 meter away
  *  
  */
  
void setup() {

  Serial.begin(9600);

  pinMode(Rtrig, OUTPUT);
  pinMode(Recho, INPUT);
  pinMode(Ltrig, OUTPUT);
  pinMode(Lecho, INPUT);
  
 

}
 long calculate_distance(int trigger, int echo)
  {
    digitalWrite(trigger, LOW); //this acts as a reset button
    if (digitalRead(trigger) == LOW)
    { digitalWrite(trigger, HIGH);} //the if statement waits until the pin is turned off before doing anything
    delayMicroseconds(10); //takes about 10 microseconds for the full pulse to be sent (fact)
    digitalWrite(trigger, LOW); // we dont want the pulse to continue pulsing, just one ten microsecond pulse
  
    timeTravel = pulseIn(echo, HIGH); //reads pin pulses, the right value is for what it's measuring (in this case we're measuring how long it takes for it to go high)
  
    distance = (timeTravel*0.034)/2; 
    /*0.034 is the speed of the pulse (sound) in microseconds (340 meters divided by 1000 milliseconds) 
    we divide by two because the echo pin serves to measure the time in which the pulse bounces off and comes back
    all we want is what happens before the bounce*/
  
    if (distance > maxRange)
    { distance = maxRange;} //so that numbers stay inside memory and neat
    if (distance < 1)
    { distance = 1;} //so that numbers stay neat and dont go negative
    
    return distance;
  }



void loop() {

  Ldist = calculate_distance(Ltrig,Lecho);
  Rdist = calculate_distance(Rtrig,Recho);
  
  

  if ((Rdist > minRange && Ldist > minRange) && (Rdist < distToTrigger && Ldist < distToTrigger)) //if object/person is between minRange to maxRange meters for both the sensors 
  {
    Serial.println("Play/Pause");
    delay(500); // so that the object/person has time to move out the way before it does the same command
  }
    
 

  //debugging (serial monitor displays distance of both sensors)
  Serial.print("l:");
  Serial.println(Ldist);
  Serial.print("r:");
  Serial.println(Rdist);
  
  delay(speedHalt); //this delay is especially important, it's the only one that's GUARANTEED to occur, halting the speed of the reads

  /* code that doesn't work:
  do
  {
    if ((Rdist > minRange && Ldist > minRange) && (Rdist < maxRange && Ldist < maxRange))
    {
      Serial.println("Play/Pause");
      delay(500);
    }
    
  }while (Rdist == maxRange && Ldist == maxRange);
  */

  //project: changing next track and prev track
  
  //next track
  if ((Ldist > minRange && Ldist < distToTrigger) && (Rdist >= 30)) //if left distance is a certain range and right isn't doing anything, and 30 just in case the sensor is reading someone's face by mistake
  { 
    delay(300); //reasonable time given for object/person to move to the other sensor if it/she/he wishes to
    Rdistold = calculate_distance(Rtrig, Recho);
    if ((Rdistold > minRange && Rdistold < distToTrigger))
    {
      Serial.println("Next"); //writes "Next" for us and the python shell module
      
      delay(1000); //so that if the object/person is still there, that means they'll want to do something else
      
      Rdist = calculate_distance(Rtrig,Recho); 
      
      /*update: continuously changing to next track*/
      while (Rdist > minRange && Rdist < Rdistold-1) //we subtract one to make sure the object/person is really moving in or not, for example if the object/person is at 1 m, there's no possible way for the person to move in
      {        
        Serial.println("Next"); 
        Rdist = calculate_distance(Rtrig, Recho);
        
        /*if (Rdist > minRange && Rdist >= Rdistold)
        {
          Rdist = calculate_distance(Rtrig,Recho);
        }*/
        delay(1000);
      } 
    }
    
  }

  //prev track
  if ((Rdist > minRange && Rdist < distToTrigger) && (Ldist >= 30)) //if right distance is a certain range and left isn't doing anything, and 30 just in case the sensor is reading someone's face by mistake
  { 
    delay(300); //reasonable time given for object/person to move to the other sensor if it/she/he wishes to
    Ldistold = calculate_distance(Ltrig, Lecho);
    if ((Ldistold > minRange && Ldistold < distToTrigger))
    {
      Serial.println("Prev"); //writes "Prev" for us and the python shell module
      delay(1000); //to check if object/person wants to do something else

      Ldist = calculate_distance(Ltrig,Lecho);
      
      while (Ldist > minRange && Ldist < Ldistold-1) //subtract 1 for assurance
      {
        Serial.println("Cprev");
        Ldist = calculate_distance(Ltrig,Lecho); //check to see if the program needs to exit the loop

        delay(1000);
      }
    }
  }

}
