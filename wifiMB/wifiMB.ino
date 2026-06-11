// benja vega
// practica n5 a

const int LED_ROJO=7;
const int POTENCIOMETRO = A0;

int valorPotenciometro;

void setup ()
{
  pinMode(LED_ROJO, OUTPUT);// CONFIGURO EL LED COMO SALIDA
}

void loop()
{
  valorPotenciometro = analogRead(POTENCIOMETRO);

  if(valorPotenciometro > 512)
  {  
    digitalWrite(LED_ROJO, HIGH);
  }
  else
  {
    digitalWrite(LED_ROJO, LOW);
  }
}


