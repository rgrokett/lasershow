/*
SPIRALLING LASERS - Arduino powered Motor Laser Show
v1.0 2016-06-02 Kinetic Design IoT 

Arduino powered three motor Laser Light Show 
Hardware design available at https://github.com/rgrokett/lasershow

Based on AP Digital Light http://www.instructables.com/id/Laser-show-for-poor-man/

Note: random() returns number between min and max - 1 
*/

// MOTOR PINS
int m1 = 3;
int m2 = 4;
int m3 = 5;

// LASER PIN
int LASER = 13;

// MOTOR SPEED 
int curspeed[] = {150, 200, 250 };
int newspeed[] = { 0, 0, 0, };

// RANGE
int minspeed = 100;
int midspeed = 200;
int maxspeed = 255;

// Other Globals
int fadeflag = 3; // start with getting new speed
int state    = 0; // slow fade or quick speed change


void setup() 
{ 
  Serial.begin(9600);  // for debugging
  Serial.println("STARTING LASER SHOW. Watch Out!");
  
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);

  pinMode(LASER, OUTPUT);
  digitalWrite(LASER, LOW);

  randomSeed(analogRead(0));

  delay(2000);  
} 
 
 
void loop() 
{ 
    // One motor can be stopped
    // Or two motors low/medium speed
    // And one motor fast
    
    // SET PWM for each motor
    analogWrite(m1, curspeed[0]);
    analogWrite(m2, curspeed[1]);
    analogWrite(m3, curspeed[2]);

    printSpeeds(); //debugging 
    
    // When all motors reach their new speed, get a new speed
    if (fadeflag >= 3)
    {
      getNewSpeed();
      fadeflag = 0;
      // Random variable delay 3 sec - 5 sec
      delay(random(3,5+1)* 1000);
      state = getState();
      digitalWrite(LASER, HIGH);
    }

    // FADE OR FAST SWITCH?
    switch(state)
    {
      case 0:  // FADING BETWEEN SPEEDS
          for (int i=0; i<=2; i++)
          {
            if (curspeed[i] < newspeed[i])
            {
              curspeed[i]++;
            }
            else
            {
              curspeed[i]--;
            }
            // Make sure speeds stay in range
            if (curspeed[i] < minspeed ) { curspeed[i] = minspeed; }
            if (curspeed[i] > maxspeed ) { curspeed[i] = maxspeed; }

            // Has a motor reached its new speed yet?
            if (curspeed[i] == newspeed[i])
            {
              fadeflag++;
            }
          } 
          delay(100);    
      break;
      
      case 1:  // FAST SWITCHING SPEEDS
          for (int i=0; i<=2; i++)
          {
            curspeed[i] = newspeed[i];
          }
          fadeflag = 3;  
          
       break;  
    }
}
// END LOOP()
 
//------- SUBROUTINES 


// GENERATE STATE SWITCHING RANDOMLY EVERY X SECONDS
int getState()
{
  // STATE = 0 or 1
  int state = 0;
  state = random(0,1+1);
  Serial.print("STATE=");
  Serial.println(state);
  return(state);
}

// GENERATE NEW SPEED VALUES
void getNewSpeed()
{
    newspeed[0] = random(minspeed,maxspeed+1);
    if (newspeed[0] < midspeed)
    {
      newspeed[1] = random(midspeed,maxspeed+1);
      newspeed[2] = random(minspeed,midspeed+1);
    }
    else
    {
      newspeed[1] = random(minspeed,midspeed+1);
      newspeed[2] = random(minspeed,midspeed+1);
    }

    // 10% chance one motor is stopped
    if (random(1,10+1) == 1)
    {
      newspeed[random(0,2+1)] = 0;
    }
}

// PRINT CURRENT SPEED FOR DEBUGGING
void printSpeeds()
{
  String msg;
  for (int i=0; i<=2;i++)
  {
    msg = msg + String("m")+i+String("=")+curspeed[i]+String(" ");
  }
  Serial.println(msg);
}



