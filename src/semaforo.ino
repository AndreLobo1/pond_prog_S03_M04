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
