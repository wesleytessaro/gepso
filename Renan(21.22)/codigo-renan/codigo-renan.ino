/**
 * Código Renan - Versão 07/12/21
 * Prof. Wesley & Wagner
 * Compilar para Arduino Mega
 * Pressão
 */
#define PINO_BUZZER 1
#define PINO_RELE_BOMBA 2
#define PINO_RELE_PNEUMATICO 3
#define PRESSAO_ALVO 68
#define TEMPO_VERIFICAR_PRESSAO_INTERNA 30000 //30 s
#define TEMPO_ANALISE_COMPLETA 60000 //60 s

const char SIMBOLO_INICIAR = 'i';
const char SIMBOLO_PARAR = 'P';
int incomingByte = 0; // para  dados recebidos na porta serial
boolean realizandoAnalise=false;
int estado=0;

int milisVerificarPressaoInterna=0;
float inicioPressaoInterna=0;
long int milisIniciado=0;
void setup() {
  pinMode(PINO_BUZZER,OUTPUT);
  pinMode(PINO_RELE_BOMBA,OUTPUT);
  pinMode(PINO_RELE_PNEUMATICO,OUTPUT);
  Serial.begin(9600);
  digitalWrite(PINO_RELE_BOMBA,LOW);
  digitalWrite(PINO_RELE_PNEUMATICO,HIGH);

}

void loop() {

  if (!realizandoAnalise){
    Serial.println(lerPressao());
   }
 if (Serial.available() > 0) {
    // lê o dado recebido:
    incomingByte = Serial.read();

    // responder o que foi recebido:
    Serial.print("Eu recebi: ");
    Serial.println((char)incomingByte);
    if ((char)incomingByte=='i'){
      realizandoAnalise=true;
       estado = 3;
      Serial.println("Abrir relê pneumático");
      digitalWrite(PINO_RELE_PNEUMATICO,LOW);
      milisIniciado=millis();
      Serial.println("Análise Iniciada:");
      milisVerificarPressaoInterna=millis ();
      Serial.println("Estado 1 - Ligar a bomba até 10 psi na câmara");
    }else if ((char)incomingByte=='p'){
      realizandoAnalise=false;
      digitalWrite(PINO_RELE_BOMBA,LOW);
      digitalWrite(PINO_RELE_PNEUMATICO,HIGH);
      Serial.println("---------Analise Finalizada---------");
    }
  }

if (realizandoAnalise){
  /*if ((millis() - milisIniciado)>TEMPO_VERIFICAR_PRESSAO_INTERNA){
    realizandoAnalise=false;
  }else{
      Serial.println(String((millis() - milisIniciado)),String(lerPressao()));
  }*/

  if (estado==1){
    Serial.println("Estado 1");
    if(lerPressao()<PRESSAO_ALVO){
      digitalWrite(PINO_RELE_BOMBA,HIGH);
    }
    else{
      digitalWrite(PINO_RELE_BOMBA,LOW);
      estado=2;
      Serial.println("Estado 2 - Aguardar 30 segundos para verificar pressão interna");
      milisVerificarPressaoInterna=millis ();
      inicioPressaoInterna=lerPressao();
    } 
  }
  else if(estado==2){
    Serial.println("Estado 2");
    if ((millis() - milisVerificarPressaoInterna)<TEMPO_VERIFICAR_PRESSAO_INTERNA){
      Serial.println("Entrou 1");
      if (abs(lerPressao()-inicioPressaoInterna)>PRESSAO_ALVO*0.10){//se variar 10%
        Serial.println("Sistema com ar vazando, suspendendo...");
        realizandoAnalise=false;
      }
    }
    else if ((millis() - milisVerificarPressaoInterna)>TEMPO_VERIFICAR_PRESSAO_INTERNA){
      Serial.println("Entrou 2");
      Serial.println("Mil"+String(millis()));
      Serial.println("milisVerificarPressaoInterna"+String(milisVerificarPressaoInterna));
      Serial.println("TEMPO_VERIFICAR_PRESSAO_INTERNA"+String(TEMPO_VERIFICAR_PRESSAO_INTERNA));
      estado = 3;
      Serial.println("Abrir relê pneumático");
      digitalWrite(PINO_RELE_PNEUMATICO,LOW);
      milisIniciado=millis();
    }
  }
  else if(estado==3){
    if ((millis() - milisIniciado)<TEMPO_ANALISE_COMPLETA){
      Serial.println(String((millis() - milisIniciado))+","+String(lerPressao()));
      delay(500);
    }
    else{
      Serial.println("----------Analise Finalizada ----------");
      realizandoAnalise=false;
      
       digitalWrite(PINO_RELE_PNEUMATICO,HIGH);
    }
  }
    
}
  

}

float lerPressao (){
float valor=analogRead(A0);
float P = ((valor / 1024) - 0.04) / 0.0012858;


return P; 
}
