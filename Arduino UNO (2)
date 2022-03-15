#include <Wire.h> // biblioteca para comunicação I2C

//Mapeamento de Hardware
#define pin_sensor 3
#define buzzer 9
#define pino_ar 13
#define pino_luz 12

// Declaração de variaveis inteiras 
int valor_luz=0, valor_tmp=0, contador_ar = 0, contador_luz=0, contador_sensor =0,k = 0;

void setup()
{
  pinMode(pino_ar, OUTPUT);
  pinMode(pino_luz, OUTPUT);
  pinMode(pin_sensor,INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent); // ISR da comunicação do Master e Slaver 
  
}

void loop()
{
  int tmp = TMP36(); 
  int luz = LDR();
  
  
  if(contador_ar == 1)
  {
    verifica_temp(tmp);
  }
  
  if(contador_luz == 1)
  {
   	verifica_luz(luz);  
  }
  
  if (contador_sensor == 1){ // Estrutura if que entra apenas quando o sensor for ativado 
    if(digitalRead(pin_sensor)==HIGH){ // Se o sensor detectar presença ele vai para nivel alto
    		tone(buzzer,150); 
    		delay(100);
      }
   }
  
  if (contador_sensor == 0){ // Estrutura if que entra apenas quando o sensor for desativado 
      noTone(buzzer);
   }
  
}

void receiveEvent(int leitura) { 
  
		int a = 0; // variavel para ler o byte enviado pelo mestre 
  	int c; // ---------- acho que pode tirar -----------
   	a = Wire.read();    // receive byte as an integer
  	
  if(a==1) { // Estrutura if para ligar o ar
    digitalWrite(pino_ar, HIGH);
    contador_ar = 0;
  }
  if(a==2){// Estrutura if para desligar o ar
    digitalWrite(pino_ar, LOW);
    contador_ar = 0;
  }
  if(a==3){// Estrutura if para setar a temperatura
    Serial.println("Escolha TMP");
    valor_tmp = Wire.read(); 
    contador_ar = 1;
  }
  if(a==4){ // Estrutura if para ligar a luz
    digitalWrite(pino_luz, HIGH);
    contador_luz = 0;
  }
  if(a==5){ // Estrutura if para desligar a luz
    digitalWrite(pino_luz, LOW);
    contador_luz = 0;
  }
  if(a==6){ // Estrutura if para escolher a quantidade de lumens
  	Serial.println("Escolha Lumens");
    valor_luz = Wire.read();
    contador_luz = 1;
  }
  
  if( a == 7){ // Estrutura if para mudar a varivel do sensor de presença,  quando o mesmo é ligado 
    contador_sensor = 1;
   }
  
  if( a == 8){ // Estrutura if para mudar a varivel do sensor de preseça, quando o mesmo é desligado
    contador_sensor = 0;
  }
  if(a==9)
  {
  	digitalWrite(pino_ar, LOW);
    digitalWrite(pino_luz, LOW);
    contador_ar = 0;
    contador_luz = 0;
    digitalWrite(pin_sensor,LOW);
    
  }
  
}

int LDR(){	// Função para ler o valor da luminosidade na porta analogica 
  
  float sensorValue = analogRead(A1);
  float trigger = map(sensorValue, 1017, 344, 0, 99);
  delay(100);
  return trigger;
  
}

int TMP36(){ // Função para ler o valor da temperatura na porta analogica 
  
  int x = analogRead(A0);
  int temp = (((x/1024.0) * (5)) - 0.5) * 100;
  return temp;
  
}

void verifica_temp(int tmp){ // função que verifica a temperatura e liga ou desliga  o ar 
  
	if(tmp >= valor_tmp){
  		digitalWrite(pino_ar, HIGH);
    	delay(100);
  	}else{
  		digitalWrite(pino_ar, LOW);
  }
}
  
void verifica_luz(int luz){ // função que verifica a luminosidade e liga ou desliga  a luz
  
	if(luz <= valor_luz){
  		digitalWrite(pino_luz, HIGH);
    	delay(100);
  	}else{
  		digitalWrite(pino_luz, LOW);
  }
  
}
