#include <Wire.h>                 //Bibioteca para lidar com a comunicação I2C utilizada pelo display Oled
#include <Adafruit_GFX.h>         //Biblioteca da Adafuit para manipular imagens e vetores para o display
#include <Adafruit_SSD1306.h>     //Biblioteca da Adafuit com funçoes para trabalhar com o display Oled
#include <MFRC522.h>//biblioteca responsável pela comunicação com o módulo RFID-RC522
#include <SPI.h> //biblioteca para comunicação do barramento SPI

#define SS_PIN 14 //Pino SS do modulo RFID
#define RST_PIN 27 //Pino Reset do modulo RFID



MFRC522 mfrc522(SS_PIN, RST_PIN); //Instancia da MRC522
Adafruit_SSD1306 display(128, 64, &Wire, -1);


void ConfiguraDisplay(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Display SSD1306 não Conectado"));
    for(;;);
  }
  display.clearDisplay(); 
  telaInicial();
  //Define o tamanho da fonte de texto utilizada para escrever no display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
 }


void telaInicial(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  //Desenha um circulo, nas coodenadas cartesianas x=48 e y=6, e com raio 6
  display.fillCircle(48, 6, 6, WHITE);
  display.fillRoundRect(42,15,  12,12,  2, WHITE);
  display.fillRoundRect(42,29,  12,12,  2, WHITE);
  display.fillRoundRect(42,43,  12,12,  2, WHITE);
  display.fillRoundRect(57,1,  12,12,  2, WHITE);
  display.fillRoundRect(57,15,  12,12,  2, WHITE);
  display.fillRoundRect(57,29,  12,12,  2, WHITE);
  display.fillRoundRect(57,43,  12,12,  2, WHITE);
  display.fillRoundRect(72,1,  12,12,  2, WHITE);
  display.fillRoundRect(72,15,  12,12,  2, WHITE);

  //Posiciona o cursor na posição x=30 e y=57
  display.setCursor(5, 57);
  //Escreve o texto na posição do cursor
  display.println("APROXIME A ETIQUETA");

  //Aplica todas as alteraçoes no display
  display.display();
}

//Se o produto for reconhecido imprime seus dados
void ProdutoReconhecido(String conteudo){

  display.clearDisplay();
  display.setTextSize(1);
  
  display.setCursor(0, 10);
  display.println("FAB: GENERAL MOTORS");

  conteudo.toUpperCase();
  display.setCursor(0, 20);
  display.print("N. SERIE:");
  display.println(conteudo);

  display.setCursor(0, 30);
  display.println("SETOR: ENGENHARIA");

  display.setCursor(0, 40);
  display.println("ULT.MANUT:03/11/2020");

  display.setCursor(0, 50);
  display.println("PROX.MANUT:03/05/2021");
  display.display();
  }

//Caso o produto não seja reconhecido apresenta item não cadastrado no display
void ProdutoNaoReconhecido(String conteudo){
  display.clearDisplay();
  display.setTextSize(1);
  
  display.setCursor(0, 10);
  display.println("ITEM NAO CADASTRADO");

  conteudo.toUpperCase();
  display.setCursor(0, 20);
  display.print("N. SERIE:");
  display.println(conteudo);
  display.display();
  }
  
void setup() {
  Serial.begin(115200);
  ConfiguraDisplay();
  SPI.begin();
  mfrc522.PCD_Init(); 
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
     return;                 // se nao tiver um cartao para ser lido recomeça o void loop
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Nao consegui ler o rfid");
    return;                  //se nao conseguir ler o cartao recomeça o void loop tambem
  }
 
  String conteudo = "";      // cria uma string
  Serial.print("id da tag :"); //imprime na serial o id do cartao
 
  for (byte i = 0; i < mfrc522.uid.size; i++){       // faz uma verificacao dos bits da memoria do cartao
     //concatena as informacoes do cartao...
     //E guarda os valores na string de conteudo para fazer as verificacoes
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  if (conteudo.substring(1) == "ca e0 a2 16"){
    ProdutoReconhecido(conteudo);
    }else{
      ProdutoNaoReconhecido(conteudo);
      }
  delay(500);
}
