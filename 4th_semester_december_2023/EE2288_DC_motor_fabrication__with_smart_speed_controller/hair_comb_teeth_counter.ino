void setup() 
{
  pinMode(A1,INPUT); // opto transducer or ir speed sensor data
  Serial.begin(9600);
}

int count=0;
int sig=0;
int iszero=0;
unsigned long prevmil=0;
unsigned long curmil=0;
void loop() {
  sig=digitalRead(A1)==1;
  while(sig){
    curmil=millis();
   if((digitalRead(A1)==1)&&(iszero==1)) // condition to count teeth only once when teeth is detected ,it makes iszero zero itself to stop further count bcoz of while loop, 
   {
     count=count+digitalRead(A1); 
     iszero=0;
     prevmil=curmil;
   } // difference between curmil and prevmil should not occur untill digitalread bcomes 1

   if((digitalRead(A1)==0)&&(curmil-prevmil)>=500)
   {
    break;
   }
    if((digitalRead(A1)==0)){iszero=1;}
  }
  if(count>0){
  Serial.println(count);
  delay(10000);
  count=0;
  }
}