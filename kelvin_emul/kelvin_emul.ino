int Temp = 0;                   // Текущее значение
const int MaxTemp = 1200;       // Максимальное значение
const char eps[9] = "=FFFFA85"; // Эпсилон

void setup() {
  Serial.begin(9600, SERIAL_8N2);

}

void loop() {
    char forsend[16] = ""; // Массив для отправки
    char StrTemp[8] = "";  // Температура строкой
    // Проверка на перебор
    if (Temp > MaxTemp) {
      Temp = 0;
    }
    // Значение температуры в строку
    sprintf(StrTemp, "%4uF\r\n", Temp);
    // Значение эпсилон
    strcat(forsend, eps);
    // + значение температуры
    strcat(forsend, StrTemp);
    // Отправляем
    Serial.print(forsend);
    Temp++;
    delay(1000);
}
