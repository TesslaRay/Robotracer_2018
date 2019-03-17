/*  Algoritmo de recuperación */
int limit = 2000;
double pos(double p1 , double p2 , double p3 , double p4 , double p5 , double p6, double y_ant)
{
  /* Función posición y algoritmo de recuperación.
   * La función convierte la entradas analogas de los sensores en entradas digitales.
   * Ademas calcula el valor de la salida entre 0 y 3 en función de la
   * posición.
   *  - p_{i}= input sensor 
   *  - y = output
   */ 
  double y;
  
  if(p1 > limit)
  {
    p1 = !linea;
  }
  else
  {
    p1 = linea;
  }
  
  if(p2 > limit)
  {
    p2 = !linea;
  }
  else
  {
    p2 = linea;
  }
  
  if(p3 > limit)
  {
    p3 = !linea;
  }
  else
  {
    p3 = linea;
  }
  
  if(p4 > limit)
  {
    p4 = !linea;
  }
  else
  {
    p4 = linea;
  }
  
  if(p5 > limit)
  {
    p5 = !linea;
  }
  else
  {
    p5 = linea;
  }
  
  if(p6 > limit)
  {
    p6 = !linea;
  }
  else
  {
    p6 = linea;
  }


  /*  Cálculo del error */
  y = (1*p1 + 0.5*p2 + 0.25*p3 - 0.25*p4 - 0.5*p5 - 1*p6)/(p1 + p2 + p3 + p4 + p5 + p6);

  /*  Detección de cruce  */
  if((p1 + p2 + p3 + p4 + p5 + p6) == 6)
  {
    cruce = true;
    tone(BUZZER, 3000, 150);
  }
  
  if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0))
  {
    y = 0;
  }

  if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (y_ant < 0))
  {
    y = -2;
  }
  
  else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (y_ant > 0))
  {
    y = 2;
  }

  return y;
}
