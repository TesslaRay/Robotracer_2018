/*  Función de sensores  */

void sensor()
{
  sensorValues[0] = analogRead(A2);
  sensorValues[1] = analogRead(A3);
  sensorValues[2] = analogRead(A4);
  sensorValues[3] = analogRead(A5);
  sensorValues[4] = analogRead(A6);
  sensorValues[5] = analogRead(A7); 
  sensorValues[6] = analogRead(A14);
  sensorValues[7] = analogRead(A9);
   
  /*  Detección de meta */
  if (sensorValues[6] < 1000)                           // cambiar < 4500 para linea blanca    cambiar > 4500 para linea negra
  {                 
    if (!s_der_ant && cruce==0)
    {
      tone(BUZZER, 2100, 200);
      s_der = true; 
      inicio++;
      if(inicio > 1 && estado == mapeo)
      {
        cont_der.Add(encoder_der.read() - posM1);
        posM1 = encoder_der.read();
        
        cont_izq.Add(encoder_izq.read() - posM2);
        posM1 = encoder_der.read();
      }
    }
    
    
    else if(!s_der_ant && cruce)         
    {
      s_der=true;
      cruce_bit = cruce_bit + 1;
    }
  
  }
  
  else
  {
    s_der = false;
  }
    
  /*  Detección de curva */
  if (sensorValues[7] < 1000)                     //  cambiar < 4500 para linea blanca   cambiar > 4500 para linea negra
  {              
    if (!s_izq_ant && cruce==0)                     //  modificar si las curvas estan muy cerca
    {       
      tone(BUZZER, 2000, 200);
      s_izq=true;
      
      if(estado == mapeo)
      {
        cont_der.Add(encoder_der.read() - posM1);
        posM1 = encoder_der.read();
        
        cont_izq.Add(encoder_izq.read() - posM2);
        posM2 = encoder_izq.read();
      }
      
      else if((estado == race_1 || estado == race_2)&&(max(encoder_der.read() - posM1, encoder_izq.read() - posM2) >= 0.8 * max(cont_der[indice], cont_izq[indice])))
      {
        posM1 = encoder_der.read();
        posM2 = encoder_izq.read();
        indice+=1;
      }
      
    }
    else if(!s_izq_ant && cruce)
    {
     s_izq=true;
     cruce_bit = cruce_bit + 1;
    }
  }
  
  else
  {
    s_izq=false;
  }
  
 /* if(s_der_ant && s_izq_ant)
  {
    s_der=false;
    s_izq=false;
  }
*/  
  s_der_ant = s_der;
  s_izq_ant = s_izq;

 if (cruce_bit == 2)
  {
    cruce_bit=0;
    cruce = 0;
  }
  
  
  Serial.print(" ");
  Serial.print(sensorValues[0]);
  Serial.print(" ");
  Serial.print(sensorValues[1]);
  Serial.print(" ");
  Serial.print(sensorValues[2]);
  Serial.print(" ");
  Serial.print(sensorValues[3]);
  Serial.print(" ");
  Serial.print(sensorValues[4]);
  Serial.print(" ");
  Serial.print(sensorValues[5]);
  Serial.print(" "); 
  Serial.print(sensorValues[6]);
  Serial.print(" ");  
  Serial.print(sensorValues[7]); 
  Serial.print(" ");
  Serial.println();
  
  
  
}
