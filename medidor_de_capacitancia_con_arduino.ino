/*-------------- Medidor de capacitancia con Arduino ---------*/

/*-- Declaración de costantes --*/
int resistencia = 10000;        //[Ohms]  La restencia del circuito RC 
int tiempoEspera = 1;

/*-- Declaración de pines --*/
int pinLecturaAnalogica = 0;    //Pin dedicado a lectura de valores analógicos
int pinCarga = 13;              //Pin dedidaco a la suministrar energía al condensador
int pinDescarga = 11;           //Pin dedicado a facilitar la descarga del condensador

/*-- Declaración de variables --*/
unsigned long tiempoInicio;     //[ms] Tiempo de inicio   
unsigned long tiempoIntervalo;  //[ms] Intervalo de tiempo entre el inicio y cuando el condensador esta cargado

float microFaradios;            //
float nanoFaradios;             //

void setup() {
  pinMode(pinCarga, OUTPUT);    //Establece el pin como de salida 
  digitalWrite(pinCarga, LOW);  //Inicia el programa sin salida en el pin de carga
  Serial.begin(9600);           //Inicio de comunicación serial (con la computadora)
}

void loop() {
  digitalWrite(pinCarga, HIGH); //Comienza a cargar el capacitor con 5 V
  tiempoInicio = millis();      //Comienza a contar los milisegundos

/*
La constante de tiempo de un condensador es el tiempo requerido para cargarlo, a través de una resistencia, 
de 0 V a aproximadamente 63.2% del voltaje directo aplicado.

Aplicando esto al circuito, la constante de tiempo del condensador en cuestión es el tiempo que tarda 
la medición del pin analógico hasta tener 63.2% del valor inicial.

En arduino, los voltajes analogicos de lecturan se representan por valores enteros de 0 (0 V) a 1023 (5 V).
Con todo esto, la constante de tiempo del condensador será el tiempo que tarda hasta que la lectura 
del pin analogico sea 63.2% * 1023 = 646.536 = 647
*/
 
while (analogRead(pinLecturaAnalogica) < 648){
  /* No se hace nada mientras que la lectura sea menor al 63.2% del voltaje suministrado */
}

tiempoIntervalo = millis() - tiempoInicio;// [ms] Se determina cuanto tiempo se tardo en cargar el condensador
microFaradios = ((float)tiempoIntervalo / resistencia)*1000; // Calcula la capcitancia
Serial.print(tiempoIntervalo);            //Imprime en pantalla el valor del intervalo del tiempo
Serial.print(" ms   ");                   //Imprime en pantalla las unidades del valor del intervalo del tiempo

if ( microFaradios > 1 )                  //Determina si son microFaradios
{
  Serial.print((long)microFaradios);      //Imprime en pantalla el valor de la capacitancia
  Serial.println(" uF");                  //Imprime en pantalla las unidades de la capacitancia
  delay(tiempoEspera);                    //Espera un tiempo para no saturar la pantalla 
}

else{                                     //Determina si son nanoFaradios
  nanoFaradios = microFaradios * 1000.0;  //Convierte microFaradios a nanoFaradios
  Serial.print((long)nanoFaradios);       //Imprime en pantalla el valor de la capacitancia
  Serial.println(" nF");                  //Imprime en pantalla las unidades de la capacitancia
  delay(tiempoEspera);                    //Espera un tiempo para no saturar la pantalla
}

digitalWrite(pinCarga, LOW);              //Detiene la cargar el condensador
pinMode(pinDescarga, OUTPUT);             //Establece el pin de decarga como uno de saiida  
digitalWrite(pinDescarga, LOW);           //Al establecer 0 V permite la decarga del capacitor

while (analogRead(pinLecturaAnalogica) > 0){
  /* No se hace nada mientras que no se haya descargado el condensador */
}

pinMode(pinDescarga, INPUT);             //Prevé al condensador para ser descargado

}
