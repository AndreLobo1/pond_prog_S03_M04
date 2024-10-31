Este repositório é destinado à atividade ponderada de programação da semana 3 do módulo 4, onde fomos desafiados a desenvolver um semáforo offline.

Abaixo, apresentamos a foto do projeto na *protoboard*:

<div align="center">

<sub>Figura 01 - Projeto semáforo *offline*</sub>
<img src="../assets/topico_3/IMG_1285.jpg">

<sub>Fonte: Material produzido pelos autores (2024)</sub>
</div>

Essa montagem foi planejada com foco na intuitividade, clareza visual e compactação. Utilizamos apenas uma pequena fração da *protoboard*, mantendo todas as conexões claramente visíveis. Para organização, usamos *jumpers* pretos para indicar conexões com o GND (negativo) e vermelhos para o positivo (5V). Para os LEDs amarelo e verde, escolhemos *jumpers* das mesmas cores dos LEDs, facilitando a identificação. Além disso, incluímos resistores para proteger os componentes. Como uma forma de ir além e já nos preparar para a próxima atividade ponderada, adicionamos um sensor de luz (LDR) e um *buzzer*.

O semáforo segue os intervalos descritos no card da atividade: o LED vermelho permanece ativo por 6 segundos, o amarelo por 2, e o verde também por 2 segundos. É importante ressaltar que, apesar de o *card* da atividade solicitar mais 2 segundos adicionais no LED verde para simular a passagem de pedestres, após alinhamento com o professor, fomos orientados a manter esse LED com 2 segundos. O ir além está no *buzzer*, que emite um som quando o LED amarelo é ativado, e no LDR, que detecta a luminosidade ambiente. Quando essa luminosidade cai abaixo de 30% do valor que foi detectado inicialmente, o modo noturno é ativado, mudando a lógica do semáforo para um piscar contínuo do LED amarelo, que se mantém assim até a desativação do modo noturno.

Abaixo, apresentamos o link do vídeo demonstrando o funcionamento:

https://drive.google.com/file/d/1lyGNfd1-dZj4lrPqMkYzt_Ru45Jv1GuM/view?usp=sharing

Seguem as tabelas de avaliação entre pares:

### Aluno avaliador: Luiza Faria Petenazzi
 
|Critério|	Contempla (Pontos)|	Contempla Parcialmente (Pontos)	|Não Contempla (Pontos)	|Observações do Avaliador|
|-|-|-|-|-|
|Montagem física com cores corretas, boa disposição dos fios e uso adequado de resistores	|Até 3	|Até 1,5	|3 | Cores corretas, boa disposição e resistores com resitencia correta para os componentes |	
|Temporização adequada conforme tempos medidos com auxílio de algum instrumento externo	|Até 3	|Até 1,5	|3 | Temporização foi cronometrada e está correta |	
|Código implementa corretamente as fases do semáforo e estrutura do código (variáveis representativas e comentários) |	Até 3|	Até 1,5 |	3 | O codigo apresenta as fases do semáforo corretamente e contém comentarios explicativos, além de variaveis representativas |	
|Ir além: Implementou um componente de extra, fez com millis() ao invés do delay() e/ou usou ponteiros no código |	Até 1 |	Até 0,5 |	1 | Implementou *millis* com sucesso |	
| Pontuação Total |------------|------------|---------->|10.0|

### Aluno avaliado: Luiza Faria Petenazzi
 
|Critério|	Contempla (Pontos)|	Contempla Parcialmente (Pontos)	|Não Contempla (Pontos)	|Observações do Avaliador|
|-|-|-|-|-|
|Montagem física com cores corretas, boa disposição dos fios e uso adequado de resistores	|Até 3	|Até 1,5	|3 |  |	
|Temporização adequada conforme tempos medidos com auxílio de algum instrumento externo	|Até 3	|Até 1,5	|3 |  |	
|Código implementa corretamente as fases do semáforo e estrutura do código (variáveis representativas e comentários) |	Até 3|	Até 1,5 |	3 | |	
|Ir além: Implementou um componente de extra, fez com millis() ao invés do delay() e/ou usou ponteiros no código |	Até 1 |	Até 0,5 |	1 |  |	
| Pontuação Total | ------------ |--------------- |-----------> |10.0|

E aqui, uma tabela dos componentes utilizados:

<div align="center">

| **Nome**                      | **Quantidade** |
|-------------------------------|----------------|
| Arduino Uno                   | 01            |
| Cabo USB A/B                  | 01            |
| LED vermelho                  | 01            |
| LED amarelo                   | 01            |
| LED verde                     | 01            |
| Buzzer                        | 01            |
| Sensor de luminosidade (LDR)  | 01            |
| Resistor                      | 04            |
| Cabo jumper macho-macho       | 08            |



</div>



O código do projeto está em src/semaforo.ino, mas também disponibilizamos logo abaixo:

```cpp
// Definindo os pinos dos LEDs e outros componentes
const uint8_t ledVermelho = 5; // Pino do LED vermelho
const uint8_t ledAmarelo = 8;  // Pino do LED amarelo
const uint8_t ledVerde = 11;   // Pino do LED verde
const uint8_t buzzer = 3;      // Pino do buzzer (som)
const uint8_t ldrPin = A0;     // Pino do sensor de luz (LDR)

// Classe para controlar o Semáforo
class Semaforo {
  private:
    uint8_t estado;            // Variável para guardar o estado atual do semáforo (0 a 3)
    unsigned long tempoAnterior; // Marca o último tempo de atualização
    bool amareloLigado;        // Indica se o LED amarelo está ligado no modo noturno
    bool modoNoturnoAtivo;     // Indica se o modo noturno foi ativado

  public:
    Semaforo() : estado(0), tempoAnterior(0), amareloLigado(false), modoNoturnoAtivo(false) {} // Inicializa o estado e o tempo

    // Método para apagar todos os LEDs e o buzzer
    void apagarTodos() {
      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledAmarelo, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(buzzer, LOW);
    }

    // Método para o funcionamento diurno do semáforo
    void iniciarModoDiurno() {
      if (modoNoturnoAtivo) {
        apagarTodos();            // Garante que todos os LEDs sejam apagados quando o modo noturno desativa
        modoNoturnoAtivo = false; // Marca que o modo noturno está desativado
      }
      unsigned long tempoAtual = millis(); // Obtém o tempo atual

      switch (estado) {
        case 0: // Estado vermelho
          if (tempoAtual - tempoAnterior >= 6000) { // Verifica se passaram 6 segundos
            digitalWrite(ledVermelho, LOW);  // Apaga o LED vermelho
            estado = 1;                      // Muda para o estado amarelo
            tempoAnterior = tempoAtual;      // Atualiza o tempo
          } else {
            digitalWrite(ledVermelho, HIGH); // Acende o LED vermelho enquanto espera
          }
          break;

        case 1: // Estado amarelo
          if (tempoAtual - tempoAnterior >= 2000) { // Tempo total de 2 segundos (200 ms de buzzer e 1800 ms de amarelo)
            digitalWrite(ledAmarelo, LOW);   // Apaga o LED amarelo
            estado = 2;                      // Muda para o estado verde
            tempoAnterior = tempoAtual;
          } else if (tempoAtual - tempoAnterior < 200) {
            digitalWrite(ledAmarelo, HIGH);  // Acende o LED amarelo
            digitalWrite(buzzer, HIGH);      // Aciona o buzzer nos primeiros 200 ms
          } else {
            digitalWrite(buzzer, LOW);       // Desliga o buzzer depois dos primeiros 200 ms
          }
          break;

        case 2: // Estado verde
          if (tempoAtual - tempoAnterior >= 2000) { // Verifica se passaram 2 segundos
            digitalWrite(ledVerde, LOW);     // Apaga o LED verde
            estado = 3;                      // Muda para o estado amarelo (antes de voltar ao vermelho)
            tempoAnterior = tempoAtual;
          } else {
            digitalWrite(ledVerde, HIGH);    // Acende o LED verde enquanto espera
          }
          break;

        case 3: // Amarelo piscando antes de voltar ao vermelho
          if (tempoAtual - tempoAnterior >= 2000) { // Tempo total de 2 segundos (200 ms de buzzer e 1800 ms de amarelo)
            digitalWrite(ledAmarelo, LOW);   // Apaga o LED amarelo
            estado = 0;                      // Volta ao estado vermelho
            tempoAnterior = tempoAtual;
          } else if (tempoAtual - tempoAnterior < 200) {
            digitalWrite(ledAmarelo, HIGH);  // Acende o LED amarelo
            digitalWrite(buzzer, HIGH);      // Aciona o buzzer nos primeiros 200 ms
          } else {
            digitalWrite(buzzer, LOW);       // Desliga o buzzer depois dos primeiros 200 ms
          }
          break;
      }
    }

    // Método para o funcionamento noturno do semáforo (LED amarelo liga por 1 segundo e desliga por 1 segundo)
    void iniciarModoNoturno() {
      if (!modoNoturnoAtivo) {
        apagarTodos();            // Apaga todos os LEDs e buzzer ao ativar o modo noturno
        modoNoturnoAtivo = true;  // Define que o modo noturno está ativo
      }

      unsigned long tempoAtual = millis(); // Obtém o tempo atual

      // Alterna entre ligado e desligado a cada 1 segundo
      if (amareloLigado && tempoAtual - tempoAnterior >= 1000) { // Se o LED está ligado e passou 1 segundo
        digitalWrite(ledAmarelo, LOW);    // Desliga o LED amarelo
        digitalWrite(buzzer, LOW);        // Desliga o buzzer
        amareloLigado = false;            // Marca como desligado
        tempoAnterior = tempoAtual;       // Atualiza o tempo
      } else if (!amareloLigado && tempoAtual - tempoAnterior >= 1000) { // Se o LED está desligado e passou 1 segundo
        digitalWrite(ledAmarelo, HIGH);   // Liga o LED amarelo
        digitalWrite(buzzer, HIGH);       // Liga o buzzer
        amareloLigado = true;             // Marca como ligado
        tempoAnterior = tempoAtual;       // Atualiza o tempo
      }
    }

    // Método para desligar o buzzer quando não é necessário
    void desligarBuzzer() {
      digitalWrite(buzzer, LOW);
    }
};

// Classe para controlar o sensor de luminosidade (LDR)
class SensorLDR {
  private:
    uint16_t valorLDR;       // Armazena o valor atual do LDR
    uint16_t limiteBaixo;    // Limite inferior de luz para modo noturno
    uint16_t limiteAlto;     // Limite superior de luz para modo diurno
    bool modoNoturno;        // Indica se o modo noturno está ativado

  public:
    SensorLDR() : valorLDR(0), limiteBaixo(0), limiteAlto(0), modoNoturno(false) {}

    // Inicializa o sensor de luminosidade e define os limites de luz
    void inicializar() {
      valorLDR = analogRead(ldrPin);      // Lê o valor inicial da luz ambiente
      limiteBaixo = valorLDR * 0.3;       // Define o limite baixo como 30% do valor inicial
      limiteAlto = valorLDR * 0.7;        // Define o limite alto como 70% do valor inicial
      Serial.begin(9600);                 // Inicializa a comunicação serial
      Serial.print("Limite LDR Baixo definido em: ");
      Serial.println(limiteBaixo);
      Serial.print("Limite LDR Alto definido em: ");
      Serial.println(limiteAlto);
    }

    // Verifica se o semáforo deve estar em modo noturno
    bool verificarModoNoturno() {
      valorLDR = analogRead(ldrPin);      // Lê o valor atual do sensor LDR
      Serial.print("Valor LDR: ");
      Serial.println(valorLDR);

      if (valorLDR < limiteBaixo && !modoNoturno) { // Se abaixo do limite baixo, ativa o modo noturno
        modoNoturno = true;
      } else if (valorLDR > limiteAlto && modoNoturno) { // Se acima do limite alto, volta para o modo diurno
        modoNoturno = false;
      }

      return modoNoturno; // Retorna o estado atual (noturno ou diurno)
    }
};

// Instancia as classes Semaforo e SensorLDR
Semaforo semaforo;
SensorLDR sensorLDR;

void setup() {
  // Configura os pinos dos LEDs e do buzzer como saídas
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Inicializa o sensor de luz
  sensorLDR.inicializar();
}

void loop() {
  // Verifica se o modo noturno está ativo
  if (sensorLDR.verificarModoNoturno()) {
    semaforo.iniciarModoNoturno(); // Inicia o modo noturno (acende o amarelo por 1 segundo e apaga por 1 segundo)
  } else {
    semaforo.desligarBuzzer();     // Desliga o buzzer
    semaforo.iniciarModoDiurno();  // Inicia o modo diurno (sequência normal)
  }
}
```

<!--                                                                                          .oMc                                                  
                                                                                           .MMMMMP
                                                                                         .MM888MM
                                                                   ....                .MM88888MP
                                                                   MMMMMMMMb.         d8MM8tt8MM
                                                                    MM88888MMMMc :' dMME8ttt8MM
                                                                     MM88tt888EMMc:dMM8E88tt88MP
                                                                      MM8ttt888EEM8MMEEE8E888MC
                                                                      MM888t8EEEM8MMEEE8t8888Mb
                                                                       "MM88888tEM8"MME88ttt88MM
                                                                        dM88ttt8EM8"MMM888ttt8MM
                                                                        MM8ttt88MM" " "MMNICKMM"
                                                                        3M88888MM"      "MMMP"
                                                                         "MNICKM" -->
