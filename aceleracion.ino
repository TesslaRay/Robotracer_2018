/*  Función de aceleración  */
void aceleracion(double Vel_final, double paso)
{


  if (Vel < 70 )
    Vel = Vel + 0.15;
  
  else 
    Vel = Vel + paso;

  if (Vel > Vel_final) 
  {
    Vel = Vel_final;  
  }
  
}
