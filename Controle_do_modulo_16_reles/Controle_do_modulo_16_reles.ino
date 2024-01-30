/* ====================================================================================================
  Programa em C/C++ : Controle do modulo de 16 relés usando os ci's 74HC595 e o 74Hc166.

  Autor: William_Sam.
  Data: 08/01/2024.
  Versão: 0.5

  Descrição: Programa feito para controlar os Ci's 74HC595 (Como saida) e 74HC166(como entrada).

  Observação: Esse programa só funcionará corretamente se tiver dois ci's 74HC595 e dois ci's 74HC166, o microcontrolador não lerá corretamente se estiver um ci 74HC166
  por causa do registrador de entrada que não é separado do registrador final dos pinos causando oscilações nas leituras.  

====================================================================================================

  Histórico das atualizações:

  Versão: 0.1  Data: 08/01/2024.
   Teste de leitura do ci 74HC166.

  Versão: 0.2  Data: 09/01/2024.
   Teste de leitura do ci 74HC166 e correção de alguns erros na leitura do ci e teste de saida do ci 74HC595.

  Versão: 0.3  Data: 12/01/2024.
   Correção de alguns erros para mandar os bytes para os registradores do ci 74HC595 e teste do codigo novo para mandar os bytes para o ci.
   ( o codigo teste para mandar os bytes para o ci não esta disponivel nesse arquivo, ainda vai ter mais testes para um futuro implemento neste arquivo).

  Versão: 0.4  Data: 18/01/2024.
   Teste do novo codigo e possivel implementação e correção de erros do codigo novo e teste de separação byte a byte de variaveis.
   ( o codigo teste para mandar os bytes para o ci não esta disponivel nesse arquivo, ainda vai ter mais testes para um futuro implemento neste arquivo).

  Versão: 0.5 Data: 23/01/2024.
   Codigo definitivo para o controle dos ci's e do modulo relé.
   (Ainda será atualizado esse arquivo).

====================================================================================================

   O esquema de ligação do 74HC595:

    Pino 16 = 5v.
    Pino 14 = Entrada de dados ( pode ser usado para conectar a saida de dados de um outro ci igual ).
    Pino 13 = GND ( habilita o funcionamento do Ci ).
    Pino 12 = É o latch ( é o pino que vai atualizar os registradores de saida ).
    Pino 11 = Clock ( desloca os 0 e 1 nos registradores primario do Ci ).
    Pino 10 = 5v. 
    Pino 9 = Saida de dados
    Pino 8 = GND.
   
   Saidas do ci: 
   
    Pino 15 = 0.
    Pino 1 = 1.
    Pino 2 = 2.
    Pino 3 = 3.
    Pino 4 = 4.
    Pino 5 = 5.
    Pino 6 = 6.
    Pino 7 = 7.

   Para ligar o segundo ci é só fazer as mesmas ligações apresentadas acima só mudando a ligação do pino 14,
    o pino 14 do segundo ci é ligado ao pino 9 do primeiro.

====================================================================================================
     
   O esquema de ligação do 74HC166:

    Pino 16 = 5v.
    Pino 15 = É o latch ( é o pino que vai atualizar os registradores de entrada ).
    Pino 13 = Saida de dados ( esta que é ligada ao Arduino ).
    Pino 9 = Vai ligado ao 5v.
    Pino 8 = GND.
    Pino 7 = Clock
    Pino 6 = Vai ao GND.
    Pino 1 = Entrada de dados ( Serve para ligar a saida de dados de outro ci igual ).

   Entradas do ci:
    
    Pino 2 = A.
    Pino 3 = B.
    Pino 4 = C.
    Pino 5 = D.
    Pino 10 = E.
    Pino 11 = F.
    Pino 12 = G.
    Pino 14 = H.

   Para ligar o segundo ci é só fazer as mesmas ligações apresentadas acima só mudando a ligação do pino 1,
    o pino 1 do segundo ci é ligado ao pino 13 do primeiro.

====================================================================================================

  Ligações no Arduino UNO/Nano:

   Pino 2 = Entrada de dados do ci 74HC595.
   Pino 3 = Atu_portas_595 do ci 74HC595.
   Pino 4 = Pin_clock_595 do ci 74HC595.

   Pino 5 = Entrada _dados do ci 74HC166.
   Pino 6 = Atu_portas_ 166 do ci 74HC166.
   Pino 7 = Pin_clock_166 do ci 74HC166.
  
  Se caso quiser usar as portas de 8 a 13 do Arduino é so mudar desse jeito:
   
   DDRD vira DDRB.
   PORTD vira PORTD.
   PIND vira PINB.

  Lembrando que os pinos irão mudar de numeração ficando assim:

   Pino 8 = 0.
   Pino 9 = 1.
   Pino 10 = 2.
   Pino 11 = 3.
   Pino 12 = 4.
   Pino 13 = 5.

  Depois dessa mudança de registrador poderá mudar os numeros que estão nos #define, exemplo: #define Saida_dados 0.
   
==================================================================================================== */

/* ==================================================================================================== */
/* ----- Mapeamento de hardware ----- */

 #define Saida_dados 2 
 #define Atu_portas_595 3
 #define Pin_clock_595 4
 
 #define Entrada_dados 5
 #define Atu_portas_166 6
 #define Pin_clock_166 7

/* ==================================================================================================== */
/* ----- Variaveis globais ----- */

 uint16_t Entrada = 0;

 bool Byte_1 = true, Byte_2 = true, Byte_3 = true, Byte_4 = true, Byte_5 = true, Byte_6 = true, Byte_7 = true, Byte_8 = true,
      Byte_9 = true, Byte_10 = true, Byte_11 = true, Byte_12 = true, Byte_13 = true, Byte_14 = true, Byte_15 = true, Byte_16 = true, Guarda_byte = false;

 bool Con_1 = false, Con_2 = false, Con_3 = false, Con_4 = false, Con_5 = false, Con_6 = false, Con_7 = false, Con_8 = false,
      Con_9 = false, Con_10 = false, Con_11 = false, Con_12 = false, Con_13 = false, Con_14 = false, Con_15 = false, Con_16 = false;

 bool Rele_1 = true, Rele_2 = true, Rele_3 = true, Rele_4 = true, Rele_5 = true, Rele_6 = true, Rele_7 = true, Rele_8 = true,
      Rele_9 = true, Rele_10 = true, Rele_11 = true, Rele_12 = true, Rele_13 = true, Rele_14 = true, Rele_15 = true, Rele_16 = true;

/* ==================================================================================================== */
/* ----- Prototipos de funções ----- */
 
 void Ci_entrada();
 void Separacao_bytes();
 void Controle();
 void Ci_saida();
 void Clock();

/* ==================================================================================================== */
/* ----- Função principal ----- */

void setup() {

           /* Saida */              /* Saida */            /* Saida */           /* Entrada */             /* Saida */            /* Saida */
 DDRD |= (1 << Saida_dados) | (1 << Atu_portas_595) | (1 << Pin_clock_595) & ~(1 << Entrada_dados) | (1 << Atu_portas_166) | (1 << Pin_clock_166); 

 while(1) { /* Cria um loop infinito */

  Ci_entrada(); /* Lê a entrada do ci 74HC595 e guarda na variavel " Entrada " */
  Separacao_bytes(); /* Separa byte a byte da variavel " Entrada " */
  Controle(); /* Faz o controle de debounce dos botões */
  Ci_saida(); /* Manda os dados para o ci 74HC595 */

  delay(5);
  
 }/* Fim do laço while */

}/* Fim da função setup */

/* ==================================================================================================== */
/* ----- Função loop infinito ----- */

void loop() {
}/* Fim da função loop */

/* ==================================================================================================== */
/* ----- Função Ci de entrada 74HC166 ----- */

void Ci_entrada(){
 
 Entrada = 0; // Limpa a variável

 PORTD &= ~(1 << Atu_portas_166); // Coloca o pino 6 em low.
 PORTD |= (1 << Pin_clock_166); // Coloca o pino 7 em high.
 PORTD &= ~(1 << Pin_clock_166); // Coloca o pino 7 em low.
 PORTD |= (1 << Atu_portas_166); // Coloca o pino 6 em high.
 Guarda_byte = (PIND & (1 << Entrada_dados)); // Pega o primeiro byte recebido.
 
 Entrada = Entrada | Guarda_byte; // Coloca na variável o primeiro byte.

 for(unsigned a = 15; a > 0; a --){ // Usando o laço for para criar repetição e pegar os 16 bytes vindo dos ci's.
  PORTD |= (1 << Pin_clock_166); // Coloca o pino 7 em high.
  PORTD &= ~(1 << Pin_clock_166); // Coloca o pino 7 em low.
  Guarda_byte = (PIND & (1 << Entrada_dados)); // Em cada repetição vai jogando byte a byte dentro da variável " Guarda_byte ".
  Entrada = Entrada | (Guarda_byte << a); // Coloca os 1 ou 0 na variável de acordo com o numero que esta no laço for.

 }/* Fim do laço for */

} /* Fim da função Ci entrada */

/* ==================================================================================================== */
/* ----- Função para separar byte a byte ----- */

void Separacao_bytes (){

/* ----- Primeiro Ci 74HC166 -----*/

  Byte_1 = ((Entrada >> 1) & 1); /* Pino A */
  Byte_2 = ((Entrada >> 2) & 1); /* Pino B */
  Byte_3 = ((Entrada >> 3) & 1); /* Pino C */
  Byte_4 = ((Entrada >> 4) & 1); /* Pino D */
  Byte_5 = ((Entrada >> 5) & 1); /* Pino E */
  Byte_6 = ((Entrada >> 6) & 1); /* Pino F */
  Byte_7 = ((Entrada >> 7) & 1); /* Pino C */
  Byte_8 = ((Entrada >> 8) & 1); /* Pino H */

/* ----- Segundo Ci 74HC166 ----- */

  Byte_9 = ((Entrada >> 9) & 1);   /* Pino A */
  Byte_10 = ((Entrada >> 10) & 1); /* Pino B */
  Byte_11 = ((Entrada >> 11) & 1); /* Pino C */
  Byte_12 = ((Entrada >> 12) & 1); /* Pino D */
  Byte_13 = ((Entrada >> 13) & 1); /* Pino E */
  Byte_14 = ((Entrada >> 14) & 1); /* Pino F */
  Byte_15 = ((Entrada >> 15) & 1); /* Pino G */
  Byte_16 = (Entrada & 1);         /* Pino H */

} /* Fim da função separação bytes */

/* ==================================================================================================== */
/* ----- Função Controle ----- */

void Controle(){

  if(!Byte_1 && Con_1 == false) Con_1 = true;

  if(Byte_1 == true && Con_1 == true){

    Rele_1 = !Rele_1;

    Con_1 = false;
  }

  if(!Byte_2 && Con_2 == false) Con_2 = true;

  if(Byte_2 == true && Con_2 == true){

    Rele_2 = !Rele_2;

    Con_2 = false;
  }

  if(!Byte_3 && Con_3 == false) Con_3 = true;

  if(Byte_3 == true && Con_3 == true){

    Rele_3 = !Rele_3;

    Con_3 = false;
  }

  if(!Byte_4 && Con_4 == false) Con_4 = true;

  if(Byte_4 == true && Con_4 == true){

    Rele_4 = !Rele_4;

    Con_4 = false;
  }

  if(!Byte_5 && Con_5 == false) Con_5 = true;

  if(Byte_5 == true && Con_5 == true){

    Rele_5 = !Rele_5;

    Con_5 = false;
  }

  if(!Byte_6 && Con_6 == false) Con_6 = true;

  if(Byte_6 == true && Con_6 == true){

    Rele_6 = !Rele_6;

    Con_6 = false;
  }

  if(!Byte_7 && Con_7 == false) Con_7 = true;

  if(Byte_7 == true && Con_7 == true){

    Rele_7 = !Rele_7;

    Con_7 = false;
  }

  if(!Byte_8 && Con_8 == false) Con_8 = true;

  if(Byte_8 == true && Con_8 == true){

    Rele_8 = !Rele_8;

    Con_8 = false;
  }

  if(!Byte_9 && Con_9 == false) Con_9 = true;

  if(Byte_9 == true && Con_9 == true){

    Rele_9 = !Rele_9;

    Con_9 = false;
  }

  if(!Byte_10 && Con_10 == false) Con_10 = true;

  if(Byte_10 == true && Con_10 == true){

    Rele_10 = !Rele_10;

    Con_10 = false;
  }

  if(!Byte_11 && Con_11 == false) Con_11 = true;

  if(Byte_11 == true && Con_11 == true){

    Rele_11 = !Rele_11;

    Con_11 = false;
  }

  if(!Byte_12 && Con_12 == false) Con_12 = true;

  if(Byte_12 == true && Con_12 == true){

    Rele_12 = !Rele_12;

    Con_12 = false;
  }

  if(!Byte_13 && Con_13 == false) Con_13 = true;

  if(Byte_13 == true && Con_13 == true){

    Rele_13 = !Rele_13;

    Con_13 = false;
  }

  if(!Byte_14 && Con_14 == false) Con_14 = true;

  if(Byte_14 == true && Con_14 == true){

    Rele_14 = !Rele_14;

    Con_14 = false;
  }

  if(!Byte_15 && Con_15 == false) Con_15 = true;

  if(Byte_15 == true && Con_15 == true){

    Rele_15 = !Rele_15;

    Con_15 = false;
  }

  if(!Byte_16 && Con_16 == false) Con_16 = true;

  if(Byte_16 == true && Con_16 == true){

    Rele_16 = !Rele_16;

    Con_16 = false;
  }

} /* Fim da função controle */

/* ==================================================================================================== */
/* ----- Função para o ci de saida 74HC595 ----- */

void Ci_saida() {
  
  if(Rele_1 == false) PORTD &= ~(1 << Saida_dados); // Se o relé 1 estiver em false liga o relé, se estiver em true desliga o relé, isto segue para cada um dos 16 relés.
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_2 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_3 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_4 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_5 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_6 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_7 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_8 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_9 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_10 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_11 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_12 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_13 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_14 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_15 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  if(Rele_16 == false) PORTD &= ~(1 << Saida_dados);
  else  PORTD |= (1 << Saida_dados);
  Clock();

  PORTD |= (1 << Atu_portas_595); // coloca o pino 3 em high.
  PORTD &= ~(1 << Atu_portas_595); // coloca o pino 3 em low.

} /* Fim da função Ci entrada */

/* ==================================================================================================== */
/* ----- Função para gerar o clock ----- */

void Clock(){

  PORTD |= (1 << Pin_clock_595); // coloca o pino 4 em high.
  PORTD &= ~(1 << Pin_clock_595); // coloca o pino 4 em low.
  
} /* Fim da função clock */
