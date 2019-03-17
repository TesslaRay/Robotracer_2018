/*  Función de desaceleración  */
void desaceleracion(double Vel_final, double paso)
{
  
  Vel = Vel - paso; 

  if (Vel < Vel_final) 
  {
    Vel = Vel_final;  
  }
}
