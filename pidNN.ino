/* PID Neural network¨*/
double x_prev = 0;        // Salida previa de Ineuron 
double u_prev = 0;        // Entrada previa de Dneuron

double u_v = 0;

/*  ----------------------------------------------  */
double PID_NN(double Kp, double Ki, double Kd)
{ 

  double theta1[2][3] = 
  {

          {  1,  1,  1},
          { -0.1, -0.1, -0.1}
          
                      };
                      
  double theta2[3][1] = 
                 {
          {Kp},
          {Ki}, 
          {Kd}                  
                 }; 
                 
  sensor();
  x = pos(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], sensorValues[4], sensorValues[5], x_ant);
  x_ant = x;

  estimated_error = kf_error.updateEstimate(x);
    
  /* Controlador PID con estructura neuronal */
  double  X[2] = {0, estimated_error};

  /* Función de neurona P */
  double a1[2];
  for (int i = 0; i < 2; i++)
  {
    a1[i] = Pneuron(X[i]);
  }

  /*  Multiplicación theta1*a1   */
  double z2[3];
  for (int i = 0; i < 3; i++)
  {
    z2[i] = 0;
    for(int j = 0; j < 2 ; j++)
    {    
      z2[i] = z2[i] + theta1[j][i]*a1[j];
    }
  }

  double a2[3];
  a2[0] = Pneuron(z2[0]);
  a2[1] = Ineuron(z2[1], x_prev);
  a2[2] = Dneuron(z2[2], u_prev);

  /* Ajuste de entradas y salidad de Ineuron y Dneuron */
  x_prev = a2[1];
  u_prev = z2[2];

  /*  Multiplicación a2*theta2   */
  double z3[1];
  for (int i = 0; i < 1; i++)
  {
    z3[i] = 0;
    for(int j = 0; j < 3; j++)
    {    
      z3[i] = z3[i] + a2[j]*theta2[j][i];
    }
  }
  
  u = Pneuron(z3[0]); 

  estimated_u = kf_u.updateEstimate(u);

  u_v = estimated_u * 1.5* Vel;
     
  return u_v;

}


/* Funciones de neuronas tipo P I D */
double threshold = 2;
double Pneuron (double u)
{
  /* Neurona tipo P
   * La función limita el valor de la salida dentro del umbral (threshold)
   *  - u = neuron input
   *  - x = neuron output
   */
  double x;
  x = u;
  if (x > threshold)
  {
    x = threshold;
  }
  if (x < -threshold)
  {
    x = -threshold;
  }
  return x;
}

double Ineuron(double u , double x_prev1)
{
  /* Neurona tipo I
   * La función limita el valor de la salida dentro del umbral (threshold).
   * Ademas la salida corresponde a la entrada + la salida previa.
   *  - u = neuron input
   *  - x = neuron output 
   *  - x_prev = previous output
   */
  double x;
  x = u + x_prev1;
  if (x > threshold)
  {
    x = threshold;
  }
  if (x < -threshold)
  {
    x = -threshold;
  }
  return x;  
}

double Dneuron(double u , double u_prev)
{ 
  /* Neurona tipo D
   * La función limita el valor de la salida dentro del umbral (threshold).
   * Ademas la salida corresponde a la entrada - la entrada previa.
   *  - u = neuron input
   *  - x = neuron output 
   *  - u_prev = previous input
   */
  double x;
  x = u - u_prev;
  if (x > threshold)
  {
    x = threshold;
  }
  if (x < -threshold)
  {
    x = -threshold;
  }
  return x;  
}
