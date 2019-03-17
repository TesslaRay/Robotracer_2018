/*  Función de motor  */
/*  Pines motor 1 (derecho)*/
#define ain1  5
#define ain2  6
#define pwm_a  4

/*  Pines motor 2 (izquierdo)*/
#define bin1  7
#define bin2  8
#define pwm_b  9
  
void motor(int Velocidad_motor1, int Velocidad_motor2)
{ 
  /*
   * Función de motor motor(M1, M2).
   * Mueve los motores con velocidad M1 y M2 con un valor entero entre 0 y 1024
   * En caso de introducir un número negativo, el motor se mueve en sentido inverso.
  */
  
  pinMode(ain1, OUTPUT); 
  pinMode(ain2, OUTPUT); 

  pinMode(bin1, OUTPUT); 
  pinMode(bin2, OUTPUT); 

  int write_resolutiton = 12;         //  Resolutión de los motores 11 = 2048 - 12 = 4096
  if (Velocidad_motor1 > 0)
  {
    analogWriteResolution(write_resolutiton);
    digitalWrite(ain1, HIGH); 
    digitalWrite(ain2, LOW);
    analogWrite(pwm_a, Velocidad_motor1); 
  }

  else if (Velocidad_motor1 < 0)
  {
    analogWriteResolution(write_resolutiton);
    digitalWrite(ain1, LOW); 
    digitalWrite(ain2, HIGH);
    analogWrite(pwm_a, -Velocidad_motor1); 
  }

  else
  {
    digitalWrite(ain1, LOW); 
    digitalWrite(ain2, LOW);
  }

  if (Velocidad_motor2 > 0)
  {
    analogWriteResolution(write_resolutiton);
    digitalWrite(bin1, HIGH); 
    digitalWrite(bin2, LOW);
    analogWrite(pwm_b, Velocidad_motor2); 
  }

  else if (Velocidad_motor2 < 0)
  {
    analogWriteResolution(write_resolutiton);
    digitalWrite(bin1, LOW); 
    digitalWrite(bin2, HIGH);
    analogWrite(pwm_b, -Velocidad_motor2); 
  }

  else
  {
    digitalWrite(bin1, LOW); 
    digitalWrite(bin2, LOW);
  }
}
