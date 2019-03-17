void maquina_estado()
{ 
  /*  Inicio máquina de estados  */
  switch (estado) 
  {
    case detenido:
      if(btn1)
      {
        estado_siguiente = detenido;
        btn1=false;
        btn2=false;
        if(meta==2){
          calculo=2;
          Vel_max = Vel_race1_low;
        }
        if(meta>1)
        {
          meta--;
        }
        else if(meta==1){
          meta=2;
        }
      }
      if(btn2 && meta==0)
      {
        
        if ( millis() > Twait  + T_espera)
        {
          estado_siguiente = mapeo;
          digitalWrite(led, LOW);
          Vel_max = Vel_mapeo;
          btn1=false;
          btn2=false;
          Twait = millis();     
        }
      }
      else if(btn2 && meta==1)
      {
        if ( millis() > Twait  + T_espera)
        {
          estado_siguiente = race_1;
          digitalWrite(led, LOW);
          btn1=false;
          btn2=false;
          Twait = millis();     
        }
      }
      
      else if(btn2 && meta==2)
      {
        if ( millis() > Twait  + T_espera)
        {
          estado_siguiente = race_2;
          digitalWrite(led, LOW);
          btn1=false;
          btn2=false;       
          Twait = millis();     
        }
      }
      else
      {
        btn1=false;
        btn2=false;
        estado_siguiente = detenido;
      }     
      break;
      
    case mapeo:
      if(btn1)
      {
        meta=0;
        estado_siguiente = detenido;
      }
      else if(s_der && !s_izq && !cruce && inicio>1)
      {
        meta=1;
        calculo=1;
        Vel_max = Vel_race1;
        estado_siguiente = detenido;   
      }
      else
      {
        estado_siguiente = mapeo;
      }
      btn1=false;
      btn2=false;
      break;
    
    case race_1:
      if(btn1)
      {
        meta=1;
        calculo=3;
        Vel_max = Vel_race1_low;
        estado_siguiente = detenido;
      }
      else if(s_der && !s_izq && !cruce && inicio>1)
      {
        meta=2;
        calculo=3;
        Vel_max = Vel_race2;
        estado_siguiente = detenido;   
      }
      else
      {
        estado_siguiente = race_1;
      }
      btn1=false;
      btn2=false;
      break;
  
    case race_2:
      if(btn1)
      {
        meta=1;
        estado_siguiente = detenido;
      }
      else if(s_der && !s_izq && !cruce && inicio>1)
      {
        meta=1;
        estado_siguiente = detenido;   
      }
      else
      {
        estado_siguiente = race_2;
      }
      btn1=false;
      btn2=false;
      break;
    
    default:
      estado_siguiente = detenido;
      meta = 0;
      btn1=false;
      btn2=false;
      break;
  }
  estado = estado_siguiente;
    
  /*  Fin máquina de estados  */
}
