// -------------------------------------------------------------------
// Ejemplo de contol de un pulsador mediante polling e interrupciones
// -------------------------------------------------------------------

int boton_flash=0;       // GPIO0 = boton flash
int estado_polling=HIGH; // por defecto HIGH (PULLUP). Cuando se pulsa se pone a LOW
int estado_int=HIGH;     // por defecto HIGH (PULLUP). Cuando se pulsa se pone a LOW

volatile int interrupcion;

unsigned long inicio_pulso_polling = 0;
volatile unsigned long ultima_int = 0;

//ultima_int = ahora;

bool poolling=false;


/*------------------  RTI  --------------------*/
// Rutina de Tratamiento de la Interrupcion (RTI)
ICACHE_RAM_ATTR void RTI() {
  int lectura=digitalRead(boton_flash);
  unsigned long ahora= millis();
  // descomentar para eliminar rebotes
  if(lectura==estado_int || ahora-ultima_int<50) 
  {//Serial.println("--------------------FILTRO ACTIVO-----------------");
  return;} // filtro rebotes 50ms
  if(lectura==LOW)
  { 
   estado_int=LOW;
   //Serial.print("Int en: ");
   //Serial.println(ahora);
   //poolling=true;
   interrupcion=millis();
  }
  else
  {
   estado_int=HIGH;
   //Serial.print("Int dura: ");
   //Serial.println(ahora-ultima_int);
   poolling=true;
   ahora=millis();
  }
   
}

/*------------------ SETUP --------------------*/
void setup() {
  pinMode(boton_flash, INPUT_PULLUP);
  // descomentar para activar interrupción
  attachInterrupt(digitalPinToInterrupt(boton_flash), RTI, CHANGE);
  Serial.println("La interrupcion se ha instalado");
  Serial.begin(115200);
  Serial.println();
  Serial.println("Pulsador listo...");
}

/*------------------- LOOP --------------------*/
void loop() {
/*  if (digitalRead(boton_flash) != estado_polling) 
  {
    if(estado_polling==HIGH) // pulsado
    {
      estado_polling=LOW;
      inicio_pulso_polling=millis();               
    }
    else
    {
      estado_polling=HIGH;
      Serial.print("Polling. Pusación desde : ");
      Serial.print(inicio_pulso_polling);
      Serial.print(" (duración: ");
      Serial.print(millis()-inicio_pulso_polling);
      Serial.println(" ms)");
    }
  }*/
int ya=millis();
  if(poolling==true){
    
    Serial.print("Se ha detectado una interrupcion en: ");
    Serial.print(ya);
    Serial.println(" ms");
    Serial.print("Y ha durado ");
    Serial.print(ya-interrupcion);
    Serial.println(" ms");
    poolling=false;
  }
  
  delay(10);
}
