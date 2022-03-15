Authors: Bruno Bueno Bronzeri ; Gabriel Mariano dos Santos Gonçalves ; Leonardo dos Santos Schmitt

#include <LiquidCrystal.h> // biblioteca do LCD
#include <Wire.h> // biblioteca comunicação I2C

int letra, coluna=0, linha=0, posicao=0, n=-1, t=0, contador=0, disp=0, flag = 0, flag_luz=0, flag_alm=0; // variaveis inteiras
char tela = ' '; //-------- talvez de para tirar---------------
char texto[33] = {}; //-------- talvez de para tirar---------------
LiquidCrystal lcd(2,4,7,9,11,13); 

byte olhoesquerdo[8]{ // cria olho esquerdo bit-a-bit
  B01110,
  B10001,
  B00100,
  B01110,
  B01110,
  B00100,
  B00000,
  B00000,
};
byte olhodireito[8]{ // cria olho direito bit-a-bit
  B01110,
  B10001,
  B00100,
  B01110,
  B01110,
  B00100,
  B00000,
  B00000,
};
byte nariz[8]{ // cria nariz bit-a-bit
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B01100,
};
byte boca1[8]{  // cria 1/3 da boca bit-a-bit
  B00000,
  B00000,
  B00000,
  B00000,
  B00010,
  B00001,
  B00000,
  B00000,
};
byte boca2[8]{//cria 2° terço da boca bit-a-bit
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
};
byte boca3[8]{ // cria 3° terço bit-a-bit
  B00000,
  B00000,
  B00000,
  B00000,
  B01000,
  B10000,
  B00000,
  B00000,
};


void setup(){
  lcd.createChar(0,olhoesquerdo); // setup do rosto
  lcd.createChar(2,olhodireito); // cria caracter do tipo olhodireito
  lcd.createChar(1,nariz);// cria caracter do tipo nariz
  lcd.createChar(3,boca1);// cria caracter do tipo boca1
  lcd.createChar(4,boca2);// cria caracter do tipo boca2
  lcd.createChar(5,boca3);	// cria caracter do tipo boca3
 
  Serial.begin(9600); //Inicia comunicação serial
  Wire.begin(); // inicia a comunicação I2C
  lcd.begin(16,2); // inicia o LCD

}

void loop(){
  
  while(t==0) //Para ligar quando for chamado
  {
    if(Serial.available() > 0 && disp == 0)
    {
    	Serial.println("Escolha a opcao");
      	t++;
      	disp++;    
    }
  }
    
  face(); // chama função de printar a face no LCD
  opc_one_lcd();// chama a funcão  
 
  while(n == 4){
  	n= -1;
  	lcd.clear();
  	boot();	
  }
  while(n==-1){
    n = Serial.read();
  } // aguarada a escolha da opção do usuário
  
  verifica(n); // chama função verifica passando paramento digitado na serial
  
}

void face(){ // função que cria a face da IA no LCD
  
   lcd.setCursor(0,0);  
   lcd.write(byte(0));   
   lcd.setCursor(1,0);  
   lcd.write(byte(1));   
   lcd.setCursor(2,0);  
   lcd.write(byte(2));  
  
   lcd.setCursor(0,1);  
   lcd.write(byte(3));  
   lcd.setCursor(1,2);   
   lcd.write(byte(4));   
   lcd.setCursor(2,3);  
   lcd.write(byte(5));
}

void verifica(int a){ // função que verifica a letra digitada pelo usuario e retorna o valor
  
switch (a){ // cases são números inteiros que serão convertidos para char ex.: 49 int = 0 char
  	case 49: // opção 1 para o ar 
    	  n=-1;
    	  Serial.println("AR");
    	  opc_one();
    	break;
    case 50:// opção 2 para a luz 
    	  n=-1;
    	  Serial.println("LUZ");
    	  opc_two();    	
    	break;
    case 51: // opção para temperatura
          n=-1; 
    	  Serial.println("ALARME");
    	  opc_three();
    	break;
   case 52: // opção para proximo, ----- acho que podemos tirar -------- 
    	    n=4;
    	    Serial.println("CNCL");
  			Wire.beginTransmission(0x08);
    		Wire.write(9);
        	Wire.endTransmission();
    	break;
    default:
    	   n=-1;
    	break;  
  }
}

void opc_one_lcd(){  //
  
	lcd.setCursor(3,0);
  	lcd.print("[1]Ar");
  	lcd.setCursor(3,1);
  	lcd.print("[2]Luz");
  	lcd.setCursor(9,0);    // ------podemos tirar eu acho ------
  	lcd.print("[3]Pres"); // ------podemos tirar eu acho ------ ou trocar cortina por um ligar alarme fodase
  	lcd.setCursor(9,1);   // ------podemos tirar eu acho ------ mete um sensor de presença e volta com o buzzer kkkkkk
  	lcd.print("[4]Cncl"); // ------podemos tirar eu acho ------
    // talvez deixar cortina sem cofnig -só on e off- e opc 4 ser um cancel que faz a IA apagar. aí ao digitar algo ela volta
}


void opc_one(){//Função que printa as opção para ar  
  opc();
  acao_ar(); // chama função 
  
}

void acao_ar(){ // ações que são utilizadas para o ar
  
  int a =-1;
  int c;
  
 while(flag == 0){
   	
  	while(n==-1){
    	n = Serial.read();
  	}
  switch (n){
  	case 49: // liga o  ar
    	Serial.println("Ar ON");
    	Wire.beginTransmission(0x08);
    	Wire.write(1);
        Wire.endTransmission();
    	n=-1;
    	break;
    case 50: // desliga o ar
    	Serial.println("Ar OFF");
    	Wire.beginTransmission(0x08);
    	Wire.write(2);
    	Wire.endTransmission();
    	n=-1;
    	break;
    case 51: // seta temperatura
   	 	Serial.println("Set TMP");
    	c = config_ar();
    	Serial.println(c);
    	Wire.beginTransmission(0x08);
    	Wire.write(3);
    	delay(300);
    	Wire.write(c);
    	Wire.endTransmission();
    	n=-1;
      opc();
    	break;
    default:
    	n=-1;
    	flag = 1;
    	break;
  }
  }
  flag = 0;
}
int config_ar(){ // função para configuração do ar-condicionado
  
    int a=-1;
    int c=0;
    char b;
  
    lcd.clear(); // limpa o LCD
    face(); // printa a face
  	lcd.setCursor(4,0); // seta o cursor
    lcd.print("Temp:");
  
    while(a==-1){
      a=Serial.read();
    } // laço para receber a temperatura desejada pelo usuário
  
    b = a;
    a = asc_int(a); // chama a função da tabela ascii para converter o número para inteiro
    c = c + (10*a);
   	lcd.setCursor(8,1);
    a=-1;
    lcd.print(b);
  
    while(a==-1){
      a=Serial.read();
    }
  
    b = a;
    a = asc_int(a);
    c = c + a;
   	lcd.setCursor(9,1);
    lcd.print(b);
    
    return c;
  }




void opc_two(){ //Função que printa as  opção para luz  
		opc();
  	acao_luz(); // chama a função de ação para a luz 
}

void acao_luz(){ // função que contem a  acão da luz 
  
  	int c;
  	
  while(flag_luz == 0){
  	while(n==-1){
      n = Serial.read();
    }
  	switch (n){
  	case 49: // opção que liga a luz
    	 Serial.println("Luz ON");
       Wire.beginTransmission(0x08);
       Wire.write(4);
       Wire.endTransmission();
    	 n=-1;
    	break;
    case 50:// opção que desliga a luz
    	 Serial.println("Luz OFF");
       Wire.beginTransmission(0x08);
       Wire.write(5);
       Wire.endTransmission();
    	 n=-1;
    	break;
    case 51: // opção que seta a luminosidade
      	c=config_luz();
    	Serial.println("Set Luminosity");
      	Serial.println(c);
      	Wire.beginTransmission(0x08);
      	Wire.write(6);
      	delay(300);
      	Wire.write(c);
      	Wire.endTransmission();
      	n=-1;
        opc();
    	break;
    default:
        flag_luz = 1;
    	  n=-1;
    	break;
  }
	}
  flag_luz = 0;
}
int config_luz() // função para configurar a luz
{
  	int a=-1;
    int c=0;
    char b;
  
    lcd.clear(); // limpa o LCD
    face(); // printa a face do LCD
  	lcd.setCursor(4,0);
    lcd.print("Lum:");
  
    while(a==-1){
      a=Serial.read();
    } // laço para receber a quantidade de luz desejada de 0-99
  
    b = a;
    a = asc_int(a); // converte valor da tabela ascii para inteiro
    c = c + (10*a);
   	lcd.setCursor(8,1);
    a=-1;
    lcd.print(b);
  
    while(a==-1){
      a=Serial.read();
    }
  
    b = a;
    a = asc_int(a);
    c = c + a;
   	lcd.setCursor(9,1);
    lcd.print(b);
    
    return c;
}

void opc_three(){  //Função que printa as opções para o alarme
	lcd.clear();
  face();
  lcd.setCursor(3,0);
  lcd.print("[1]On");
  lcd.setCursor(3,1);
  lcd.print("[2]Off");
  lcd.setCursor(9,0);
  lcd.print("[3]Back");
  acao_alarme();// chama função alarme 
}

void acao_alarme(){
  while(flag_alm==0){
  	while(n==-1){
      n = Serial.read();
    }
  switch (n){
  	case 49:
    	Serial.println("ALM ON");
    	Wire.beginTransmission(0x08);
    	Wire.write(7);
      Wire.endTransmission();
    	n=-1;
    	break;
    case 50:
    	Serial.println("ALM OFF");
    	Wire.beginTransmission(0x08);
    	Wire.write(8);
      Wire.endTransmission();
    	n=-1;
    	break;    
    default:
      flag_alm= 1;
    	n=-1;
    	break;
  }
  }
  flag_alm = 0;
}




int asc_int(int a) // função que converte valores ascii para inteiro
{
  	int b = 48, c = 0;
		c = a - b;
		return c;
}

void boot(){

   while(n==-1){
    n = Serial.read();
  } // aguarada a escolha da opção do usuário
  
  face(); // chama função de printar a face no LCD
  opc_one_lcd();// chama a funcão

  verifica(n); // chama função verifica passando paramento digitado na serial
  
}
void opc()
{
  lcd.clear();
  face();
  lcd.setCursor(3,0);
  lcd.print("[1]On");
  lcd.setCursor(3,1);
  lcd.print("[2]Off");
  lcd.setCursor(9,0);
  lcd.print("[3]Conf");
  lcd.setCursor(9,1);
  lcd.print("[4]Back");
}
  
