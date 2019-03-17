/*  Vector de velocidad  */
void vector_velocidad(int velocidad)
{
  float radio=0;
  for(int i=0; i < int(cont_der.Count());i++)
  {
    radio=5.75*(cont_der[i] + cont_izq[i])/(abs(cont_der[i] - cont_izq[i]));
    if(radio > 30*curvatura)
    {
      velocidad_race.Add(150);  
    }
    else
    {
      velocidad_race.Add(Vel_race1);
    }

  }
}
