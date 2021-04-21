/*
  Скетч разработан 30.11.2018 Wirekraken доработан AzamirDEV Fox в 19.04.2021г.
*/
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <FastLED.h>
//библиотека времени DS1302 скачать можно в ардуино
#include <ThreeWire.h>
#include <RtcDS1302.h>

//пины для подключения модуля часов
ThreeWire myWire(12, 13, 14); // пины по последоваттельности: DAT/IO, CLK/SCLK, RST/CE  соответственно:///5,6,7 пин у esp8266
RtcDS1302<ThreeWire> Rtc(myWire);
#define countof(a) (sizeof(a) / sizeof(a[0]))
const char data[] = "what time is it";
int lastSecond = 0;
int step = 0;
void updateMainColor(int colorR, int colorG, int colorB);

//адресса переменных для сохранения данных
#define EE_LED_MODE 0
#define EE_LED_BRIGHT 1
#define EE_LED_SPEED 2
#define EE_LED_COLOR_R 3
#define EE_LED_COLOR_G 4
#define EE_LED_COLOR_B 5
//запись элемента code

const char *ssid = "ssid";             // имя вашей сети
const char *password = "password"; // пароль вашей сети

IPAddress Ip(192, 168, 1, 200);     // IP-адрес для ESP
IPAddress Gateway(192, 168, 1, 1);  // IP-адрес шлюза (роутера)
IPAddress Subnet(255, 255, 255, 0); // маска подсети, диапазон IP-адресов в локальной сети

#define LED_COUNT 15 // число пикселей в ленте
#define LED_DT 2     // пин, куда подключен DIN ленты (номера пинов ESP8266 нифига не совпадает с Arduino)

uint8_t bright = 25; // яркость (0 - 255)
uint8_t ledMode = 0; // эффект (0 - 29)

uint8_t flag = 1; // флаг отмены эффекта

CRGBArray<LED_COUNT> leds;

uint8_t baseDelayValue = 20; // Базовая задержка
uint8_t delayValue = 20;     // автовычисление задержки по скорости и базовой задержке(можно не изменять)
uint8_t stepValue = 10;      // шаг по пикселям
uint8_t hueValue = 0;        // тон цвета
boolean detectUser = false;

// инициализация websocket на 81 порту
WebSocketsServer webSocket(81);
ESP8266WebServer server(80);

//класс расписания для эффектов по времени

class Schedule
{
public:
  int hour = 0, min = 0;
  int colorR = 0, colorB = 0, colorG = 0;
  int ledModeS = 0, ledBright = 0, ledSpeed = 0;
  Schedule(int _hour, int _min, int _colorR, int _colorG, int _colorB, int _ledMode, int _ledBright, int _ledSpeed)
  {
    hour = _hour;
    min = _min;
    colorR = _colorR;
    colorG = _colorG;
    colorB = _colorB;
    ledModeS = _ledMode;
    ledBright = _ledBright;
    ledSpeed = _ledSpeed;
  }
  boolean scheduleFound(int _hour, int _min)
  {
    if (hour == _hour && min == _min)
    {
      editGlobalParam();
      return true;
    }
    else
    {
      return false;
    }
  }
  void editGlobalParam()
  {
    Serial.println("Schedule editGlobalMode time found");
    ledMode = ledModeS;
    bright = ledBright;
    delayValue = ledSpeed;
    updateMainColor(colorR, colorG, colorB);
  }
  void testData()
  {
    Serial.print("ScheduleData: ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(min);
    Serial.println();
    Serial.print("RGB: ");
    Serial.print(colorR);
    Serial.print(" ");
    Serial.print(colorG);
    Serial.print(" ");
    Serial.print(colorB);
    Serial.println();
    Serial.print("LED_MODE/BRIGHT/SPEED: ");
    Serial.print(ledModeS);
    Serial.print(" ");
    Serial.print(ledBright);
    Serial.print(" ");
    Serial.print(ledSpeed);
    Serial.println();
  }
};

void setup()
{
  EEPROM.begin(512);
  Serial.begin(9600);
  addWatch(); //добавление модуля часов(на базе DS1302) используется для смены режима по расписанию

  LEDS.setBrightness(bright);

  LEDS.addLeds<WS2812B, LED_DT, GRB>(leds, LED_COUNT); // настройки для вашей ленты (ленты на WS2811, WS2812, WS2812B)
  updateColor(0, 0, 0);
  LEDS.show();

  WiFi.config(Ip, Gateway, Subnet);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //установка данных ленты до выключения ленты(вдруг свет отключили или еще что то там...)
  ledMode = EEPROM.read(EE_LED_MODE);
  bright = EEPROM.read(EE_LED_BRIGHT);
  delayValue = EEPROM.read(EE_LED_SPEED);
  updateMainColor(EEPROM.read(EE_LED_COLOR_R), EEPROM.read(EE_LED_COLOR_G), EEPROM.read(EE_LED_COLOR_B));

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      detectUser = true;
    server.send(404, "text/plain", "FileNotFound");
  });

  server.begin();

  SPIFFS.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void startSchedule(Schedule *sheduleArray)
{
  RtcDateTime now = Rtc.GetDateTime();
  if (lastSecond != now.Second())
  {
    lastSecond = now.Second();
    step++;
  }
  if (step > 55)
  {
    step = 0;
    Serial.print(now.Hour());
    Serial.print(":");
    Serial.print(now.Minute());
    Serial.print(":");
    Serial.println(now.Second());
    for (byte i = 0; i <= sizeof(sheduleArray); i++)
    {
      sheduleArray[i].scheduleFound(now.Hour(), now.Minute());
    }
  }
};

//Создание своего расписания для отображения по времени (часы(0-24), минуты(0-59), цвет красный (0-255), зеленый (0-255), синий (0-255),режим эффекта(0-40), яркость (0-255), скорость(20-400))
Schedule myShedule1(8, 32, 251, 255, 251, 2, 251, 225);
Schedule myShedule2(8, 33, 252, 254, 252, 3, 242, 224);
Schedule myShedule3(8, 34, 253, 253, 253, 4, 233, 223);
Schedule myShedule4(8, 35, 254, 252, 254, 5, 224, 222);
Schedule myShedule5(8, 36, 255, 251, 255, 6, 215, 221);
//вставляем все наши расписания в один массив и чтобы работало в функции loop() запускаем функцию startSchedule(); вставив в параметр наш массив.
Schedule sheduleArray[] = {myShedule1, myShedule2, myShedule3, myShedule4, myShedule5};

void loop()
{
  //обработка входящих запросов HTTP или WebSockets
  webSocket.loop();
  server.handleClient();
  if (!detectUser)
  {
    ledEffect(ledMode);
  }
  startSchedule(sheduleArray); //запуск расписания можно отключить если не пользоваться
  /*просмотр сохраненных данных
  Serial.print("eeProm LEDMODE: ");
  Serial.print(EEPROM.read(EE_LED_MODE));
  Serial.print(" eeProm SPEED: ");
  Serial.print(EEPROM.read(EE_LED_SPEED));
  Serial.print(" eeProm BRIGHT: ");
  Serial.print(EEPROM.read(EE_LED_BRIGHT));
  Serial.print(" eeProm R: ");
  Serial.print(EEPROM.read(EE_LED_COLOR_R));
  Serial.print(" eeProm G: ");
  Serial.print(EEPROM.read(EE_LED_COLOR_G));
  Serial.print(" eeProm B: ");
  Serial.println(EEPROM.read(EE_LED_COLOR_B));
  */
}
//функция добавления модуля часов
void addWatch()
{
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (!Rtc.IsDateTimeValid())
  {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }
  if (Rtc.GetIsWriteProtected())
  {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }
  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }

  /* comment out on a second run to see that the info is stored long term */
  // Store something in memory on the RTC
  uint8_t count = sizeof(data);
  uint8_t written = Rtc.SetMemory((const uint8_t *)data, count); // this includes a null terminator for the string
  if (written != count)
  {
    Serial.print("something didn't match, count = ");
    Serial.print(count, DEC);
    Serial.print(", written = ");
    Serial.print(written, DEC);
    Serial.println();
  }
}

//функция обработки входящих сообщений
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{

  if (type == WStype_CONNECTED)
  {
    IPAddress ip = webSocket.remoteIP(num);

    String message = String("Connected");
    webSocket.broadcastTXT(message); // отправляем последнее значение всем клиентам при подключении
  }

  if (type == WStype_TEXT)
  {
    String data;
    for (int x = 0; x < length; x++)
    {
      if (!isdigit(payload[x]))
        continue;
      data += (char)payload[x];
    }

    if (payload[0] == 'B')
    {
      flag = 0;
      Serial.print("Bright: ");
      bright = data.toInt();
      Serial.println(data);
      LEDS.setBrightness(bright);
      EEPROM.write(EE_LED_BRIGHT, data.toInt());
      EEPROM.commit();
    }
    else if (payload[0] == 'S')
    {
      flag = 0;
      Serial.print("Speed: ");
      if (data.toInt() != 0)
      {
        delayValue = (baseDelayValue * data.toInt()) / 20;
      }
      Serial.print(data);
      Serial.print(" delay: ");
      Serial.println(delayValue);
      EEPROM.write(EE_LED_SPEED, delayValue);
      EEPROM.commit();
    }
    else if (payload[0] == 'F')
    {
      flag = 0;
      Serial.print("Function: ");
      ledMode = data.toInt();
      Serial.println(data);
      ledEffect(ledMode);
      EEPROM.write(EE_LED_MODE, data.toInt());
      EEPROM.commit();
    }
    else if (payload[0] == '#')
    {

      if (!flag)
      {
        Serial.print("flag : ");
        Serial.println(flag);
        ledMode = flag;
        ledEffect(ledMode);
        flag = 1;
      }
      else
      {
        //преобразуем в 24 битное цветовое число
        uint32_t rgb = (uint32_t)strtol((const char *)&payload[1], NULL, 16);

        //преобразуем 24 бит по 8 бит на канал
        uint8_t r = abs(0 + (rgb >> 16) & 0xFF);
        uint8_t g = abs(0 + (rgb >> 8) & 0xFF);
        uint8_t b = abs(0 + (rgb >> 0) & 0xFF);

        updateMainColor(r, g, b);
        oneColor();
        EEPROM.write(EE_LED_COLOR_R, r);
        EEPROM.write(EE_LED_COLOR_G, g);
        EEPROM.write(EE_LED_COLOR_B, b);
        EEPROM.commit();

        LEDS.show();
      }
    }
  }
}

// функция эффектов
void ledEffect(int ledMode)
{
  detectUser = false;
  switch (ledMode)
  {
  case 0:
    updateColor(0, 0, 0);
    break;
  case 1:
    rainbow_fade(20);
    break;
  case 2:
    rainbow_loop(20);
    break;
  case 3:
    new_rainbow_loop(5);
    break;
  case 4:
    random_march(40);
    break;
  case 5:
    rgb_propeller(25);
    break;
  case 6:
    rotatingRedBlue(40);
    hueValue = 0;
    break;
  case 7:
    Fire(55, 120, 15);
    break;
  case 8:
    blueFire(55, 250, 15);
    break;
  case 9:
    random_burst(20);
    break;
  case 10:
    flicker(20);
    break;
  case 11:
    random_color_pop(35);
    break;
  case 12:
    Sparkle(255, 255, 255, delayValue);
    break;
  case 13:
    color_bounce(20);
    break;
  case 14:
    color_bounceFADE(20);
    break;
  case 15:
    red_blue_bounce(40);
    hueValue = 0;
    break;
  case 16:
    rainbow_vertical(50);
    stepValue = 15;
    break;
  case 17:
    matrix(50);
    break;

  // тяжелые эффекты
  case 18:
    rwb_march();
    delayValue = 80;
    break;
  case 19:
    flame();
    break;
  case 20:
    theaterChase(255, 0, 0, 50);
    break;
  case 21:
    Strobe(255, 255, 255, 10, delayValue, 1000);
    delayValue = 100;
    break;
  case 22:
    policeBlinker();
    delayValue = 25;
    break;
  case 23:
    kitt();
    delayValue = 100;
    break;
  case 24:
    rule30();
    delayValue = 100;
    break;
  case 25:
    fade_vertical();
    delayValue = 60;
    hueValue = 180;
    break;
  case 26:
    fadeToCenter();
    break;
  case 27:
    runnerChameleon();
    break;
  case 28:
    blende(10);
    break;
  case 29:
    blende_2(10);
    break;
  case 30: //новые эффекты
    oneColor();
    break;
  case 31:
    runColor(20);
    break;
  case 32:
    meteorRain(20);
    break;
  case 33:
    TwinkleRandom(5, false);
    break;
  case 34:
    FadeInOut();
    break;
  case 35:
    RGBLoop();
    break;
  case 36:
    colorWipe(10);
    break;
  case 37:
    colorWipe2(10);
    break;
  case 38:
    longColorWipe(10);
    break;
  case 39:
    impulse(490);
    break;
  case 40:
    heartBeats();
  }
}

// функция получения типа файла
String getContentType(String filename)
{
  if (server.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

// функция поиска файла в файловой системе
bool handleFileRead(String path)
{
#ifdef DEBUG
  Serial.println("handleFileRead: " + path);
#endif
  if (path.endsWith("/"))
    path += "index.html";
  if (SPIFFS.exists(path))
  {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, getContentType(path));
    file.close();
    return true;
  }
  return false;
}
