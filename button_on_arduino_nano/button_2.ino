#include <SPI.h>                                               // Подключаем библиотеку для работы с шиной SPI.
#include <nRF24L01.h>                                          // Подключаем файл настроек из библиотеки RF24.
#include <RF24.h>   
#include "printf.h"                                           // Подключаем библиотеку для работы с nRF24L01+.
RF24     radio(9, 10);
const int ledPin1 = 4;  
const int ledPin2 = 5;                                       // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов модуля (CE, SS).
int      myData[5];
const char message[] = "LED_ON";                                            // Объявляем массив для хранения и передачи данных (до 32 байт включительно).
bool flag = false;
uint32_t btnTimer = 0;
bool send = false;
bool take = false;

void setup(){
  pinMode(3, INPUT_PULLUP); // Контакт для кнопки.
  Serial.begin(9600); // Канал для передачи сообщений на компьютер.
  pinMode(ledPin1, OUTPUT); // Красный светодиод.
  pinMode(ledPin2, OUTPUT); // Зеленый светодиод.
  digitalWrite(ledPin1, HIGH); // Начальное состояние светодиода - выкл.
  digitalWrite(ledPin2, HIGH); // Начальное состояние светодиода - выкл.
  radio.begin           (); // Инициируем работу модуля nRF24L01+.
  radio.setChannel      (27); // Указываем канал передачи данных (от 0 до 125), 27 - значит передача данных осуществляется на частоте 2,427 ГГц.
  radio.setDataRate     (RF24_1MBPS); // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек.
  radio.setPALevel      (RF24_PA_HIGH); // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm).
  radio.openReadingPipe (1, 0xAABBCCDD11LL); 
  radio.openWritingPipe (0xAABBCCDD33LL);
  radio.startListening  ();
}

void loop(){
  // тест связи
  while (send == false && take == false) {
    // Проверка отправки
    radio.stopListening();
    if( radio.write(&message, sizeof(message)) ){                // Если указанное количество байт массива myData было доставлено приёмнику, то ...
      send = true;
    }
    // Проверка принятия
    radio.startListening();
    if(radio.available()){
    char message[32] = "";
    radio.read( &message, sizeof(message) );
    Serial.println(message) ;
      if(strcmp(message, "LED_ON") == 0){ 
      Serial.println("press3") ;
      Serial.println(message) ;
      take = true;
    }
    }   
    digitalWrite(ledPin1, LOW); // Включаем светодиод
    delay(100); // Ждем 5 секунд
    digitalWrite(ledPin1, HIGH);
    delay(100);
  }
  // Флаг для кнопки.
  bool btnState = !digitalRead(3);
  // Код нажатия кнопки.
  if (btnState && !flag && millis() - btnTimer > 100) {
    flag = true;
    btnTimer = millis();
    Serial.println("press");
    bool sent = false;
    const char message[] = "LED_ON";
    radio.stopListening();
    sent = radio.write(&message, sizeof(message)); // Попытка отправки
    Serial.println("Attempting to send LED_ON");
    radio.startListening();
    Serial.print(sent);
    uint32_t startTime = millis();
    while (millis() - startTime < 5000) {
      digitalWrite(ledPin2, LOW);
      delay(500);
      digitalWrite(ledPin2, HIGH);
      delay(500);
    }
  }
  // Код проверки залипания кнопки.
  if (btnState && flag && millis() - btnTimer > 500) {
    btnTimer = millis();
    Serial.println("press hold");
  }
  // Принудительное выключение светодиода, если кнопка не нажата.
  if (!btnState && flag && millis() - btnTimer > 500) {
    flag = false;
    btnTimer = millis();
    //Serial.println("release");
    digitalWrite(ledPin2, HIGH); // Выключаем светодиод
  }

  // Чтение радио, прием сообщений.
  if(radio.available()){
    char message[32] = "";
    radio.read( &message, sizeof(message) );
    Serial.println(message) ;
      if(strcmp(message, "LED_ON") == 0){ 
      Serial.println("press3") ;
      Serial.println(message) ;
      digitalWrite(ledPin1, LOW); // Включаем светодиод.
      delay(4900); // Ждем 4.9 секунды.
      digitalWrite(ledPin1, HIGH);
      
    }   
  }
} 