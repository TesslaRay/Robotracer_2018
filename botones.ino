/*  Función de botones  */
void boton1()
{ 
   if ( millis() > T0  + 250)
   {   
    tone(BUZZER, 1800, 100);
    btn1 = true;
    T0 = millis();
   }
}

void boton2()
{ 
   if ( millis() > T0  + 250)
   {   
    tone(BUZZER, 1800, 100);
    btn2 = true;
    T0 = millis();
    Twait = millis();
   }
}
