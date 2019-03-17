#define ENCODER_OPTIMIZE_INTERRUPTS

#include <ListLib.h>
#include <Encoder.h>

#include <SimpleKalmanFilter.h>

/*  Filtro Kalman   */
SimpleKalmanFilter kf_der = SimpleKalmanFilter(10, 10, 0.7);
SimpleKalmanFilter kf_izq = SimpleKalmanFilter(10, 10, 0.7);

SimpleKalmanFilter kf_error = SimpleKalmanFilter(0.2, 0.2, 1);
SimpleKalmanFilter kf_u = SimpleKalmanFilter(0.2, 0.2, 0.5);

SimpleKalmanFilter kf_u_der = SimpleKalmanFilter(0.2, 0.2, 0.7);
SimpleKalmanFilter kf_u_izq = SimpleKalmanFilter(0.2, 0.2, 0.7);

double estimated_error;
double estimated_u;

#define detenido 0
#define mapeo 1
#define race_1 2
#define race_2 3
#define linea 1                   //  1 es linea banca y 0 es linea negra 
#define curvatura 10              //  radio de curvatura critico 10[cm]


/*PID adaptativo*/
double Kp = 0.0;
double Ki = 0.0;
double Kd = 0.0;

/*PID MAPEO*/
double Kp_map = 5.0;
double Ki_map = 0.00985;
double Kd_map = 490.0;

/*PID RACE1*/
#define kp_race1 5.0
#define ki_race1 0.00985
#define kd_race1 490.0

/*PID RACE1 LOW*/
#define kp_race1_low 5.0
#define ki_race1_low 0.00985
#define kd_race1_low 490.0

/*PID RACE2*/
#define kp_race2 5.0
#define ki_race2 0.00985
#define kd_race2 490.0

#define BT Serial1                //Rx 0 - Tx 1
#define bluetooh_pin 12

#define led 13
#define BUZZER 3
#define BOTON_1 2
#define BOTON_2 11

double largo=0;
double Vel_mapeo = 65.0;
double Vel_race1 = 75.0;
double Vel_race1_low = 70.0;
double Vel_race2 = 150.0;

double Vel = 0.0;
double Vel_max = 0.0;

double Fs = 500.0;
double h = 1.0 / Fs;

unsigned long T0 = 0.0;
unsigned long T1 = 0.0;
unsigned long T2 = 0.0;
unsigned long Twait = 0.0;

Encoder encoder_der(14, 10);
Encoder encoder_izq(22, 15);

List<int> cont_der;
List<int> cont_izq;
List<int> velocidad_race;

/* Variables */
double u = 0;                        //  Salida del PID
double x = 0;

double M1 = 0;
double M2 = 0;

float x_ant = 0;

/*  Sensores  */
float sensorValues[8];

/*  Variables adicionales  */
bool freno = false;

float k = 0;                        //  Constante de frenado
unsigned long rebote_meta = 0;
unsigned long rebote_curva = 0;

/*Variables parpedeo led (negro)*/
int led_counter = 0;
bool led_flag = false;
int led_speed = 10000;

//-------------------------------------------------------------------
/*  Variables de los sensores   */
bool s_der = false;
bool s_izq = false;
bool s_der_ant = false;
bool s_izq_ant = false;

int inicio = 0;
bool btn1 = false;
bool btn2 = false;
bool cruce = false;
int cruce_bit = 0;
int calculo = 0; // para que calcule una vez el vector de velocidad race 1

/*  Variables utilizadas en la máquina de estados   */
int estado = 0;
int estado_siguiente = 0;

int T_espera = 2000;

int indice = 0;

/*  Variable meta   */
int meta = 0;

/*  Variables necesarias para el recibo de datos  */
const byte numChars = 40;
char receivedChars[numChars];
char tempChars[numChars];

char messageFromPC[numChars] = {0};
int integerFromPC = 0;
double doubleFromPC1 = 0.0;
double doubleFromPC2 = 0.0;
double doubleFromPC3 = 0.0;
double doubleFromPC4 = 0.0;
double doubleFromPC5 = 0.0;
double doubleFromPC6 = 0.0;

boolean newData = false;

/*Variables Encoder*/
double posM1 = 0;
double posM2 = 0;

/*  Variables globales controlador de velocidad */
double x_act_izq = 0.0;
double x_act_der = 0.0;

double x_ant_der = 0.0;
double x_ant_izq = 0.0;

double error_der = 0.0;
double error_izq = 0.0;

double pos_der;
double pos_anterior_der = 0;

double pos_izq;
double pos_anterior_izq = 0;

void setup()
{
  BT.begin(9600);
  Serial.begin(9600);

  analogReadResolution(13);

  attachInterrupt(digitalPinToInterrupt(BOTON_1), boton1, RISING);
  attachInterrupt(digitalPinToInterrupt(BOTON_2), boton2, RISING);

  pinMode(led, OUTPUT);

  pinMode(bluetooh_pin, OUTPUT);
  digitalWrite(bluetooh_pin, HIGH);
}

void loop()
{
  if (micros() > T1  + h * 1000000)
  {
    led_counter = (led_counter + 1) % led_speed;
    if (!led_counter)
      led_flag = !led_flag;

    /*  Recepción de datos desde el bluetooh  */
    recvWithStartEndMarkers();
    if (newData == true)
    {
      strcpy(tempChars, receivedChars);
      parseData();
      assignData();
      newData = false;
    }
    /*    ------    */

    if (btn1 == true || btn2 == true || s_der == true)
    {
      maquina_estado();
    }

    if (estado == detenido)
    {
      x = 0;
      u = 0;
      error_der=0;
      error_izq=0;
      x_ant_izq=0;
      x_ant_der=0;

      x_act_izq = 0;
      x_act_der = 0;
      pos_der = encoder_der.read()*2*PI/118;
      pos_izq = encoder_izq.read()*2*PI/118;
      pos_anterior_der = pos_der;
      pos_anterior_izq = pos_izq;
      
      posM1 = encoder_der.read();
      posM2 = encoder_izq.read();

      indice = 0;
      inicio = 0;
      cruce = 0;
      s_der = false;
      s_izq = false;
      s_der_ant = false;
      s_izq_ant = false;

      if (calculo != 0)
      {
        velocidad_race.RemoveRange(0, velocidad_race.Count());
        vector_velocidad(Vel_max);
        if (calculo == 1)
        {
          Kp = kp_race1;
          Ki = ki_race1;
          Kd = kd_race1;
        }
        else if (calculo == 2)
        {
          Kp = kp_race1_low;
          Ki = ki_race1_low;
          Kd = kd_race1_low;
        }
        else if (calculo == 3)
        {
          Kp = 2.0;
          Ki = 0.01;
          Kd = 200.0;
        }

        calculo = 0;
      }

      if (meta == 0)
      {
        cont_der.RemoveRange(0, cont_der.Count());  //  Reinicia las cuentas del encoder derecho
        cont_izq.RemoveRange(0, cont_izq.Count());  //  Reinicia las cuentas del encoder izquierdo
        digitalWrite(led, HIGH);
      }

      else if (meta == 1)
      {
        led_speed = 100;
        if (led_flag)
          digitalWrite(led, HIGH);
        else
          digitalWrite(led, LOW);
      }

      else if (meta == 2)
      {
        led_speed = 50;
        if (led_flag)
          digitalWrite(led, HIGH);
        else
          digitalWrite(led, LOW);
      }

      if (Vel > 0)
      {
        desaceleracion(0, 0.5);

        u = PID_NN(Kp_map, Ki_map, Kd_map);
        velocidad(Vel - u , Vel + u);
      }

      else
        motor(0, 0);

    }

    if (estado == mapeo)
    {
      digitalWrite(led, LOW);

      aceleracion(Vel_mapeo, 0.2);

      u = PID_NN(Kp_map, Ki_map, Kd_map);
      velocidad(Vel - u, Vel + u);
    }

    else if (estado == race_1 )
    {
      digitalWrite(led, LOW);
      
      aceleracion(Vel_race1, 0.2);

      u = PID_NN(Kp_map, Ki_map, Kd_map);
      velocidad(Vel - u, Vel + u);
    }

    else if (estado == race_2)
    {
      digitalWrite(led, LOW);
      
      if ((velocidad_race[indice] != Vel_mapeo) && (max(encoder_der.read() - posM1, encoder_izq.read() - posM2) < 0.7 * max(cont_der[indice], cont_izq[indice])))
      {
        aceleracion(velocidad_race[indice], 0.3);
        
        u = PID_NN(Kp, Ki, Kd);
        velocidad(Vel - u, Vel + u);
        k=Vel;
      }

      else if ((velocidad_race[indice] != Vel_mapeo) && (max(encoder_der.read() - posM1, encoder_izq.read() - posM2) >= 0.7 * max(cont_der[indice], cont_izq[indice])))
      {
        desaceleracion(Vel_mapeo - 20,1);
        u = PID_NN(Kp_map, Ki_map, Kd_map);
        velocidad(Vel - u, Vel + u);
      }

      else
      {
        if (Vel < Vel_race1)
          aceleracion(Vel_race1, 0.1);
        else
          desaceleracion(Vel_race1, 0.1);

        u = PID_NN(Kp_map, Ki_map, Kd_map);
        velocidad(Vel - u, Vel + u);
      }
    }

    T1 = micros();
  }

}
