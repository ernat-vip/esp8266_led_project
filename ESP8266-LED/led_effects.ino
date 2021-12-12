int idex = 0;     //индекс текущего пикселя
uint8_t ihue = 0; // тон цвета
uint8_t mainColorR = 0, mainColorG = 0, mainColorB = 0;
uint8_t saturationVal = 255;        // насыщенность
uint8_t ibright = 0;                //значение яркости
int TOP_INDEX = int(LED_COUNT / 2); // получаем середину ленты
int EVENODD = LED_COUNT % 2;        //флаг проверки четности
int bouncedirection = 0;            //флаг для color_bounce()
uint8_t ledsX[LED_COUNT][3];        //массив для сохранения случайных значений пикселя
int myStepValue = 3;                //шаги для уменьшения

void updateMainColor(int r, int g, int b)
{
  mainColorR = r;
  mainColorG = g;
  mainColorB = b;
  Serial.print("updateMainColor: R-");
  Serial.print(r);
  Serial.print(" G-");
  Serial.print(g);
  Serial.print(" B-");
  Serial.print(b);
  Serial.println();
}

void updateColor(uint8_t r, uint8_t g, uint8_t b)
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i].setRGB(r, g, b);
  }
}

void oneColor()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i].setRGB(mainColorR, mainColorG, mainColorB);
  }
  LEDS.show();
  delay(15);
}

//плавная смена цветов
void rainbow_fade(uint8_t minDelay)
{
  ihue++;
  if (ihue > 255)
  {
    ihue = 0;
  }
  for (int idex = 0; idex < LED_COUNT; idex++)
  {
    leds[idex] = CHSV(ihue, saturationVal, 255);
  }
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

// крутящаяся радуга
void rainbow_loop(uint8_t minDelay)
{
  idex++;
  ihue = ihue + stepValue;
  if (idex >= LED_COUNT)
  {
    idex = 0;
  }
  if (ihue > 255)
  {
    ihue = 0;
  }
  leds[idex] = CHSV(ihue, saturationVal, 255);
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

// случайная смена цветов
void random_burst(uint8_t minDelay)
{
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  leds[idex] = CHSV(ihue, saturationVal, 255);
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

// бегающий пиксель
void color_bounce(uint8_t minDelay)
{
  if (bouncedirection == 0)
  {
    idex = idex + 1;
    if (idex == LED_COUNT)
    {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if (bouncedirection == 1)
  {
    idex = idex - 1;
    if (idex == 0)
    {
      bouncedirection = 0;
    }
  }
  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i == idex)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB); //CHSV(HUE_RED, saturationVal, 255);
    }
    else
    {
      leds[i] = CHSV(0, 0, 0);
    }
  }
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

//бегающий паровозик пикселей
void color_bounceFADE(uint8_t minDelay)
{
  if (bouncedirection == 0)
  {
    idex = idex + 1;
    if (idex == LED_COUNT)
    {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if (bouncedirection == 1)
  {
    idex = idex - 1;
    if (idex == 0)
    {
      bouncedirection = 0;
    }
  }
  int iL1 = adjacent_cw(idex);
  int iL2 = adjacent_cw(iL1);
  int iL3 = adjacent_cw(iL2);
  int iR1 = adjacent_ccw(idex);
  int iR2 = adjacent_ccw(iR1);
  int iR3 = adjacent_ccw(iR2);

  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i == idex)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB); // = CHSV(hueValue, saturationVal, 255);
    }
    else if (i == iL1)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB);
      leds[i].fadeToBlackBy(100); // = CHSV(hueValue, saturationVal, 150);
    }
    else if (i == iL2)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB);
      leds[i].fadeToBlackBy(175); // = CHSV(hueValue, saturationVal, 80);
    }
    else if (i == iL3)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB);
      leds[i].fadeToBlackBy(230); // = CHSV(hueValue, saturationVal, 20);
    }
    else if (i == iR1)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB);
      leds[i].fadeToBlackBy(100); // = CHSV(hueValue, saturationVal, 150);
    }
    else if (i == iR2)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB);
      leds[i].fadeToBlackBy(175); // = CHSV(hueValue, saturationVal, 80);
    }
    else if (i == iR3)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB);
      leds[i].fadeToBlackBy(230); // = CHSV(hueValue, saturationVal, 20);
    }
    else
    {
      leds[i] = CHSV(0, 0, 0);
    }
  }
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

// вращение красного и синего
void red_blue_bounce(uint8_t minDelay)
{
  idex++;
  if (idex >= LED_COUNT)
  {
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (hueValue + 160) % 255;

  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i == idexR)
    {
      leds[i] = CHSV(hueValue, saturationVal, 255);
    }
    else if (i == idexB)
    {
      leds[i] = CHSV(thathue, saturationVal, 255);
    }
    else
    {
      leds[i] = CHSV(0, 0, 0);
    }
  }
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

int antipodal_index(int i)
{
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX)
  {
    iN = (i + TOP_INDEX) % LED_COUNT;
  }
  return iN;
}

// вращение красного/синего
void rotatingRedBlue(uint8_t minDelay)
{
  idex++;
  if (idex >= LED_COUNT)
  {
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (hueValue + 160) % 255;
  leds[idexR] = CHSV(hueValue, saturationVal, 255);
  leds[idexB] = CHSV(thathue, saturationVal, 255);
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

// случайный стробоскоп
void flicker(uint8_t minDelay)
{
  int random_bright = random(0, 255);
  int random_delay = random(10, 100);
  int random_bool = random(0, random_bright);
  if (random_bool < 10)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      leds[i].setRGB(mainColorR, mainColorG, mainColorB); // = CHSV(hueValue, saturationVal, ibright);
      leds[i].fadeToBlackBy(random_bright);
    }
    LEDS.show();
    delay(random_delay);
  }
}

// плавная смена яркости
void fade_vertical()
{
  idex++;
  if (idex > TOP_INDEX)
  {
    idex = 0;
  }
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  ibright = ibright + 2;//10;
  if(myStepValue>0){
    myStepValue--;
    }else{
      ihue++;
      myStepValue=3;
      }
  
  if (ihue > 255)
  {
    ihue = 0;
  }
  if (ibright < 5)
  {
    ibright = 5;
  }
  if (ibright > 255)
  {
    ibright = 0;
  }
  leds[idexA] = CHSV(ihue, 255, ibright);//.setRGB(mainColorR, mainColorG, mainColorB); // = CHSV(hueValue, saturationVal, ibright);
  leds[idexA].fadeToBlackBy(ibright);
  leds[idexB] = CHSV(ihue, 255, ibright);//.setRGB(mainColorR, mainColorG, mainColorB); // = CHSV(hueValue, saturationVal, ibright);
  leds[idexB].fadeToBlackBy(ibright);
  LEDS.show();
  delay(delayValue);
}

//служебная функция
int horizontal_index(int i)
{
  if (i == 0)
  {
    return 0;
  }
  if (i == TOP_INDEX && EVENODD == 1)
  {
    return TOP_INDEX + 1;
  }
  if (i == TOP_INDEX && EVENODD == 0)
  {
    return TOP_INDEX;
  }
  return LED_COUNT - i;
}

//служебная функция
void random_red()
{
  int temprand;
  for (int i = 0; i < LED_COUNT; i++)
  {
    temprand = random(0, 100);
    if (temprand > 50)
    {
      leds[i].r = 255;
    }
    if (temprand <= 50)
    {
      leds[i].r = 0;
    }
    leds[i].b = 0;
    leds[i].g = 0;
  }
  LEDS.show();
}

//безумие красных светодиодов
void rule30()
{
  int y = LED_COUNT / 10;
  for (int i = 0; i < y; i++)
  {
    int myRandom = random(0, LED_COUNT);
    if (leds[myRandom].r == 0 && leds[myRandom].g == 0 && leds[myRandom].b == 0)
    {
      leds[myRandom].setRGB(mainColorR, mainColorG, mainColorB);
    }
    else
    {
      leds[myRandom].setRGB(0, 0, 0);
    }
  }
  LEDS.show();
  delay(delayValue);
}
int adjacent_cw(int i)
{
  int r;
  if (i < LED_COUNT - 1)
  {
    r = i + 1;
  }
  else
  {
    r = 0;
  }
  return r;
}
int adjacent_ccw(int i)
{
  int r;
  if (i > 0)
  {
    r = i - 1;
  }
  else
  {
    r = LED_COUNT - 1;
  }
  return r;
}

// безумие случайных цветов
void random_march(uint8_t minDelay)
{
  copy_led_array();
  int iCCW;
  leds[0] = CHSV(random(0, 255), 255, 255);
  for (int idex = 1; idex < LED_COUNT; idex++)
  {
    iCCW = adjacent_ccw(idex);
    leds[idex].r = ledsX[iCCW][0];
    leds[idex].g = ledsX[iCCW][1];
    leds[idex].b = ledsX[iCCW][2];
  }
  LEDS.show();
  delay(1);
}
void copy_led_array()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    ledsX[i][0] = leds[i].r;
    ledsX[i][1] = leds[i].g;
    ledsX[i][2] = leds[i].b;
  }
}

//белый синий красный бегут по кругу
void rwb_march()
{
  copy_led_array();
  int iCCW;
  idex++;
  if (idex > 2)
  {
    idex = 0;
  }
  switch (idex)
  {
  case 0:
    leds[0].r = 255;
    leds[0].g = 0;
    leds[0].b = 0;
    break;
  case 1:
    leds[0].r = 255;
    leds[0].g = 255;
    leds[0].b = 255;
    break;
  case 2:
    leds[0].r = 0;
    leds[0].g = 0;
    leds[0].b = 255;
    break;
  }
  for (int i = 1; i < LED_COUNT; i++)
  {
    iCCW = adjacent_ccw(i);
    leds[i].r = ledsX[iCCW][0];
    leds[i].g = ledsX[iCCW][1];
    leds[i].b = ledsX[iCCW][2];
  }
  LEDS.show();
  delay(delayValue);
}

// эффект пламени
void flame()
{
  int idelay = random(0, 35);
  float hmin = 0.1;
  float hmax = 45.0;
  float hdif = hmax - hmin;
  int randtemp = random(0, 3);
  float hinc = (hdif / float(TOP_INDEX)) + randtemp;
  int ihue = hmin;
  for (int i = 0; i <= TOP_INDEX; i++)
  {
    ihue = ihue + hinc;
    leds[i] = CHSV(ihue, saturationVal, 255);
    int ih = horizontal_index(i);
    leds[ih] = CHSV(ihue, saturationVal, 255);
    leds[TOP_INDEX].r = 255;
    leds[TOP_INDEX].g = 255;
    leds[TOP_INDEX].b = 255;
    LEDS.show();
    delay(idelay);
  }
}

// радуга
void rainbow_vertical(uint8_t minDelay)
{
  idex++;
  if (idex > TOP_INDEX)
  {
    idex = 0;
  }
  ihue = ihue + stepValue;
  if (ihue > 255)
  {
    ihue = 0;
  }
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  leds[idexA] = CHSV(ihue, saturationVal, 255);
  leds[idexB] = CHSV(ihue, saturationVal, 255);
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

// безумие случайных вспышек
void random_color_pop(uint8_t minDelay)
{
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  updateColor(0, 0, 0);
  leds[idex] = CHSV(ihue, saturationVal, 255);
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

// полицейская мигалка
void policeBlinker()
{
  int hueValue = 0;
  int thathue = (hueValue + 160) % 255;
  for (int x = 0; x < 5; x++)
  {
    for (int i = 0; i < TOP_INDEX; i++)
    {
      leds[i] = CHSV(hueValue, saturationVal, 255);
    }
    LEDS.show();
    delay(delayValue);
    updateColor(0, 0, 0);
    LEDS.show();
    delay(delayValue);
  }
  for (int x = 0; x < 5; x++)
  {
    for (int i = TOP_INDEX; i < LED_COUNT; i++)
    {
      leds[i] = CHSV(thathue, saturationVal, 255);
    }
    LEDS.show();
    delay(delayValue);
    updateColor(0, 0, 0);
    LEDS.show();
    delay(delayValue);
  }
}

void rgb_propeller(uint8_t minDelay)
{ // пропеллер
  idex++;
  int ghue = (hueValue + 80) % 255;
  int bhue = (hueValue + 160) % 255;
  int N3 = int(LED_COUNT / 3);
  int N6 = int(LED_COUNT / 6);
  int N12 = int(LED_COUNT / 12);

  for (int i = 0; i < N3; i++)
  {
    int j0 = (idex + i + LED_COUNT - N12) % LED_COUNT;
    int j1 = (j0 + N3) % LED_COUNT;
    int j2 = (j1 + N3) % LED_COUNT;
    leds[j0] = CHSV(hueValue, saturationVal, 255);
    leds[j1] = CHSV(ghue, saturationVal, 255);
    leds[j2] = CHSV(bhue, saturationVal, 255);
  }
  LEDS.show();
  if (delayValue < minDelay)
  {
    delay(minDelay);
  }
  else
  {
    delay(delayValue);
  }
}

//случайные вспышки красного
void kitt()
{
  int rand = random(0, TOP_INDEX);
  for (int i = 0; i < rand; i++)
  {
    leds[TOP_INDEX + i].setRGB(mainColorR, mainColorG, mainColorB); // = CHSV(hueValue, saturationVal, 255);
    leds[TOP_INDEX - i].setRGB(mainColorR, mainColorG, mainColorB); // = CHSV(hueValue, saturationVal, 255);
    LEDS.show();
    delay(delayValue / rand);
  }
  for (int i = rand; i > 0; i--)
  {
    leds[TOP_INDEX + i] = CHSV(hueValue, saturationVal, 0);
    leds[TOP_INDEX - i] = CHSV(hueValue, saturationVal, 0);
    LEDS.show();
    delay(delayValue / rand);
  }
}

// зеленые бегают по кругу случайно
void matrix(uint8_t minDelay)
{
  int rand = random(0, 100);
  if (rand > 90)
  {
    leds[0].setRGB(mainColorR, mainColorG, mainColorB); // = CHSV(hueValue, saturationVal, 255);
  }
  else
  {
    leds[0] = CHSV(hueValue, saturationVal, 0);
  }
  copy_led_array();
  for (int i = 1; i < LED_COUNT; i++)
  {
    leds[i].r = ledsX[i - 1][0];
    leds[i].g = ledsX[i - 1][1];
    leds[i].b = ledsX[i - 1][2];
  }
  LEDS.show();
    delay(delayValue);
}

// плавная вращающаяся радуга
void new_rainbow_loop(uint8_t minDelay)
{
  if(bouncedirection>0){
        idex-=3;
        if(idex<10){bouncedirection=0;}
        }else{
        idex+=3;
        if(idex>16000){bouncedirection=1;}}
  fill_rainbow(leds, LED_COUNT, idex, 255);
  LEDS.show();
  delay(delayValue);
}

void new_rainbow_loop2(uint8_t minDelay)//ADD
{
      if(bouncedirection>0){
        idex-=3;
        if(idex<10){bouncedirection=0;}
        }else{
        idex+=3;
        if(idex>800){bouncedirection=1;}}
        switch(myStepValue){
          case 1:if(ihue<255){ihue++;}else{myStepValue=2;}
          break;
          case 2:if(ihue>252){ihue--;}else{myStepValue=1;}
          break;
          default:
          myStepValue=1;
          ihue = 252;
          break;
          }
  fill_rainbow(leds, LED_COUNT, 600, ihue);
  LEDS.show();
  delay(200);
}

void setPixel(int Pixel, byte red, byte green, byte blue)
{
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}
//служебная функция
void setAll(byte red, byte green, byte blue)
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    setPixel(i, red, green, blue);
  }
  FastLED.show();
}

//линейный огонь
void Fire(int Cooling, int Sparking, int SpeedDelay)
{
  static byte heat[LED_COUNT];
  int cooldown;

  for (int i = 0; i < LED_COUNT; i++)
  {
    cooldown = random(0, ((Cooling * 10) / LED_COUNT) + 2);

    if (cooldown > heat[i])
    {
      heat[i] = 0;
    }
    else
    {
      heat[i] = heat[i] - cooldown;
    }
  }

  for (int k = LED_COUNT - 1; k >= 2; k--)
  {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  if (random(255) < Sparking)
  {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
  }

  for (int j = 0; j < LED_COUNT; j++)
  {
    setPixelHeatColor(j, heat[j]);
  }

  FastLED.show();
  if (delayValue < SpeedDelay)
  {
    delay(SpeedDelay);
  }
  else
  {
    delay(delayValue);
  }
}

void setPixelHeatColor(int Pixel, byte temperature)
{
  byte t192 = round((temperature / 255.0) * 191);
  byte heatramp = t192 & 0x3F;
  heatramp <<= 2;

  if (t192 > 0x80)
  {
    setPixel(Pixel, 255, 255, heatramp);
  }
  else if (t192 > 0x40)
  {
    setPixel(Pixel, 255, heatramp, 0);
  }
  else
  {
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = ((LED_COUNT - EyeSize) / 2); i >= 0; i--)
  {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(LED_COUNT - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(LED_COUNT - i - j, red, green, blue);
    }
    setPixel(LED_COUNT - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    FastLED.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = 0; i <= ((LED_COUNT - EyeSize) / 2); i++)
  {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(LED_COUNT - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(LED_COUNT - i - j, red, green, blue);
    }
    setPixel(LED_COUNT - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    FastLED.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = 0; i < LED_COUNT - EyeSize - 2; i++)
  {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    FastLED.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

// случайные вспышки белого цвета
void Sparkle(byte red, byte green, byte blue, int SpeedDelay)
{
  int Pixel = random(LED_COUNT);
  setPixel(Pixel, mainColorR, mainColorG, mainColorB);
  FastLED.show();
  delay(SpeedDelay);
  setPixel(Pixel, 0, 0, 0);
}

// бегущие каждые 3 пикселя
void theaterChase(byte red, byte green, byte blue, int SpeedDelay)
{
   for (int i = 0; i < LED_COUNT; i++)
      {
  if(myStepValue<0){
    myStepValue=3;
    setPixel(i, mainColorR, mainColorG, mainColorB);
  }else{myStepValue--;
  setPixel(i, 0, 0, 0);
  }
      }
      FastLED.show();
  delay(delayValue);
  
/*
  for (int j = 0; j < 10; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      for (int i = 0; i < LED_COUNT; i = i + 3)
      {
        setPixel(i + q, mainColorR, mainColorG, mainColorB);
        //setPixel(i + q, red, green, blue);
      }
      FastLED.show();
      delay(delayValue);
      for (int i = 0; i < LED_COUNT; i++)
      {
        setPixel(i + q, 0, 0, 0);
      }
    }
  }
  */
}

// стробоскопический эффект
void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause)
{
  for (int j = 0; j < StrobeCount; j++)
  {
    setAll(mainColorR, mainColorG, mainColorB);
    FastLED.show();
    delay(FlashDelay);
    setAll(0, 0, 0);
    FastLED.show();
    delay(FlashDelay);
  }

  delay(EndPause);
}

//голубой линейный огонь
void blueFire(int Cooling, int Sparking, int SpeedDelay)
{
  static byte heat[LED_COUNT];
  int cooldown;

  for (int i = 0; i < LED_COUNT; i++)
  {
    cooldown = random(0, ((Cooling * 10) / LED_COUNT) + 2);

    if (cooldown > heat[i])
    {
      heat[i] = 0;
    }
    else
    {
      heat[i] = heat[i] - cooldown;
    }
  }
  for (int k = LED_COUNT - 1; k >= 2; k--)
  {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  if (random(255) < Sparking)
  {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
  }
  for (int j = 0; j < LED_COUNT; j++)
  {
    setPixelHeatColorBlue(j, heat[j]);
  }
  FastLED.show();
  if (delayValue < SpeedDelay)
  {
    delay(SpeedDelay);
  }
  else
  {
    delay(delayValue);
  }
}

void setPixelHeatColorBlue(int Pixel, byte temperature)
{
  byte t192 = round((temperature / 255.0) * 191);
  byte heatramp = t192 & 0x03;
  heatramp <<= 2;

  if (t192 > 0x03)
  {
    setPixel(Pixel, heatramp, 255, 255);
  }
  else if (t192 > 0x40)
  {
    setPixel(Pixel, 255, heatramp, 0);
  }
  else
  {
    setPixel(Pixel, 0, 0, heatramp);
  }
}

//расплывающие цвета
void fadeToCenter()
{
    if(idex > LED_COUNT / 2){idex=0;}else{
    leds.fadeToBlackBy(2);
    leds[idex] = CHSV(ihue++, 255, 255);
    leds(LED_COUNT / 2, LED_COUNT - 1) = leds(LED_COUNT / 2 - 1, 0);
    idex++;}
    FastLED.delay(delayValue);
  /*static uint8_t hue;
  for (int i = 0; i < LED_COUNT / 2; i++)
  {
    leds.fadeToBlackBy(40);
    leds[i] = CHSV(ihue++, 255, 255);
    leds(LED_COUNT / 2, LED_COUNT - 1) = leds(LED_COUNT / 2 - 1, 0);
    FastLED.delay(delayValue);
  }*/
}

//бегающий по кругу паровозик
void runnerChameleon()
{
  if(idex+2>LED_COUNT){idex=0;}else{
        idex++;
        }
  leds[idex] = CHSV(ihue++, 255, 255);
    FastLED.show();
    //leds[idex] = CRGB::Black;
        fadeall();
    delay(delayValue);
  /*
  static uint8_t hue = 0;
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    leds[i] = CRGB::Black;
    //    fadeall();
    delay(10);
  }*/
}

void blende()
{
  
  if(idex>=0 && idex<=LED_COUNT){ 
    int maxIdex=0;
    int minIdex = 0;
    switch(bouncedirection){ 
      case 0:
      if(idex+2>LED_COUNT){bouncedirection=1;}else{
        maxIdex = idex+1;
        }
      for (; idex < maxIdex; idex++)
  {
    leds[idex] = CHSV(ihue++, 255, 255);
    FastLED.show();
    //leds[idex] = CRGB::Black;
    fadeall();
    delay(delayValue);

    Serial.print("SW 1 - :");
      Serial.println(idex);
  }
  break;
  case 1:
 if(idex-2<0){bouncedirection=0;}else{
        maxIdex = idex-1;
        }
  for (; idex > maxIdex; idex--)
  {
    leds[idex] = CHSV(ihue++, 255, 255);
    FastLED.show();
    //     leds[i] = CRGB::Black;
    fadeall();
    delay(delayValue);

    Serial.print("SW 2 -- :");
      Serial.println(idex);
  }
  break;
  }
    }
    if(idex>LED_COUNT){
      idex=123;
      Serial.print("ELSE - idex/flag:");
      Serial.println(idex);
      }
 
  
  /*
  static uint8_t hue = 0;
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    //     leds[i] = CRGB::Black;
    fadeall();
      delay(delayValue);
  }

  for (int i = (LED_COUNT)-1; i >= 0; i--)
  {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    //     leds[i] = CRGB::Black;
    fadeall();
      delay(delayValue);
  }*/
}

void blende_2()
{
  if(idex>=0 && idex<=LED_COUNT){ 
    int maxIdex=0;
    int minIdex = 0;
    switch(bouncedirection){ 
      case 0:
      if(idex+2>LED_COUNT){bouncedirection=1;}else{
        maxIdex = idex+1;
        }
      for (; idex < maxIdex; idex++)
  {
    leds[idex] = CHSV(ihue++, 255, 255);
    FastLED.show();
    leds[idex] = CRGB::Black;
    fadeall();
    delay(delayValue);

    Serial.print("SW 1 - :");
      Serial.println(idex);
  }
  break;
  case 1:
 if(idex-2<0){bouncedirection=0;}else{
        maxIdex = idex-1;
        }
  for (; idex > maxIdex; idex--)
  {
    leds[idex] = CHSV(ihue++, 255, 255);
    FastLED.show();
    //     leds[i] = CRGB::Black;
    fadeall();
    delay(delayValue);

    Serial.print("SW 2 -- :");
      Serial.println(idex);
  }
  break;
  }
    }
    if(idex>LED_COUNT){
      idex=123;
      Serial.print("ELSE - idex/flag:");
      Serial.println(idex);
      }
  // DOWANDA
  /*for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i] = CHSV(ihue++, 255, 255);
    FastLED.show();
    leds[i] = CRGB::Black;
    fadeall();
    delay(delayValue);
  }

  for (int i = (LED_COUNT)-1; i >= 0; i--)
  {
    leds[i] = CHSV(ihue++, 255, 255);
    FastLED.show();
    //     leds[i] = CRGB::Black;
    fadeall();
    delay(delayValue);
  }*/
}
//служебная функция
void fadeall()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i].nscale8(254);//(250); 0-255 maxx_value
  }
}
//мои новые эффекты
void heartBeats()
{
  float r, g, b;
  if(ihue>3 | ihue<0){ihue=0;idex=0;}
  else{
    switch(ihue){
      case 0:
      if(idex<=140){
        Serial.println("0");
        delay(2);
    r = (idex / 256.0) * mainColorR;
    g = (idex / 256.0) * mainColorG;
    b = (idex / 256.0) * mainColorB;
    updateColor(r, g, b);
    FastLED.show();
    if(idex>=140){delay(150);ihue++;}
    idex++;
        }
      break;
      case 1:
      if(idex>=15){
        Serial.println("1");
        delay(7);
    r = (idex / 256.0) * mainColorR;
    g = (idex / 256.0) * mainColorG;
    b = (idex / 256.0) * mainColorB;
    updateColor(r, g, b);
    FastLED.show();
    if(idex<=15){delay(150);ihue++;}
    idex--;idex--;
    }
      break;
      case 2:
      if(idex<=255){
        Serial.println("2");
        delay(2);
    r = (idex / 256.0) * mainColorR;
    g = (idex / 256.0) * mainColorG;
    b = (idex / 256.0) * mainColorB;
    updateColor(r, g, b);
    FastLED.show();
    if(idex>=255){delay(150);ihue++;}
    idex++;
        }
      break;
      case 3:
      if(idex>=0){
        Serial.println("3");
        delay(1);
    r = (idex / 256.0) * mainColorR;
    g = (idex / 256.0) * mainColorG;
    b = (idex / 256.0) * mainColorB;
    updateColor(r, g, b);
    FastLED.show();
    if(idex<=0){delay(600);ihue=0;break;}
    idex--;idex--;
        }
      break;
      }
    }
}

void impulse(int SpeedDelay)
{
  float r, g, b;
  int MIN_LED_COUNT = 30;
  int MAX_LED_COUNT = 256;
    if(idex>=MIN_LED_COUNT && idex<=MAX_LED_COUNT){
      switch (bouncedirection)
    {
    case 1:
      idex++;
    delay(2);
    r = (idex / 256.0) * mainColorR;
    g = (idex / 256.0) * mainColorG;
    b = (idex / 256.0) * mainColorB;
    updateColor(r, g, b);
    FastLED.show();
    if(idex==MAX_LED_COUNT){delay(150);}
      break;
    case 0:
      idex--;idex--;
      delay(10);
      r = (idex / 256.0) * mainColorR;
      g = (idex / 256.0) * mainColorG;
      b = (idex / 256.0) * mainColorB;
      updateColor(r, g, b);
      FastLED.show();
      break;
    }
      }else{
        switch (bouncedirection)
    {
    case 1:
      idex=MAX_LED_COUNT;
      bouncedirection=0;
      break;
    case 0:
      idex=MIN_LED_COUNT;
      bouncedirection=1;
      break;
    }
        }
    delay(delayValue);
}

void runColor(int SpeedDelay)
{
  int Position = 0;

  for (int i = 0; i < 6; i++)
  {
    Position++;
    for (int i = 0; i < LED_COUNT; i++)
    {
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255) * mainColorR,
               ((sin(i + Position) * 127 + 128) / 255) * mainColorG,
               ((sin(i + Position) * 127 + 128) / 255) * mainColorB);
    }

    FastLED.show();
    delay(5);
  }
}

void meteorRainRGB(int SpeedDelay)
{
if(idex>=0 && idex <=LED_COUNT)
  {
    // fade brightness all LEDs one step
    for (int j = 0; j < LED_COUNT; j++)
    {
       
      if ((!true) || (random(10) > 5))
      {
        fadeToBlack(j, random(5));
      }
    }
    // draw meteor
    for (int j = 0; j < random(10); j++)
    {
      if ((idex - j < LED_COUNT) && (idex - j >= 0))
      {
        //setPixel(idex - j, mainColorR, mainColorG, mainColorB);
        if(myStepValue>0){
          ihue = ihue++;
          myStepValue--;
          }else{
            myStepValue=5;
            ihue++;
            }
        if(++ihue>255){ihue=0;}
        leds[idex] = CHSV(ihue, saturationVal, 255);
      }
    }
    if(idex==LED_COUNT){
      idex=1;}else{
      idex++;
      }
    FastLED.show();
    delay(delayValue);
  }else{
    idex=0;
    }
}
void meteorRain(int SpeedDelay)
{
if(idex>=0 && idex <=LED_COUNT)
  {
    // fade brightness all LEDs one step
    for (int j = 0; j < LED_COUNT; j++)
    {
      if ((!true) || (random(10) > 5))
      {
        fadeToBlack(j, random(5));
      }
    }
    // draw meteor
    for (int j = 0; j < random(10); j++)
    {
      if ((idex - j < LED_COUNT) && (idex - j >= 0))
      {
        setPixel(idex - j, mainColorR, mainColorG, mainColorB);
      }
    }

    if(idex==LED_COUNT){idex=0;}else{
      idex++;
      }
    FastLED.show();
    delay(delayValue);
  }else{
    idex=0;
    }
}

// used by meteorrain
void fadeToBlack(int ledNo, byte fadeValue)
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int)r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int)g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int)b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, r, g, b);
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[ledNo].fadeToBlackBy(fadeValue);
#endif
}

void TwinkleRandom(int SpeedDelay, boolean OnlyOne)
{
    int MIN_LED_COUNT = 200;
    int MAX_LED_COUNT = 210;
    if(idex>=MIN_LED_COUNT && idex<=MAX_LED_COUNT){
      switch (bouncedirection)
    {
    case 1:
      idex++;
      Serial.print(" idex+: ");
      Serial.println(idex);
      setPixel(random(LED_COUNT), random(0, 255), random(0, 255), random(0, 255));
      break;
    case 0:
      idex--;
      Serial.print(" idex-: ");
      Serial.println(idex);
      setPixel(random(LED_COUNT), 0, 0, 0);
       setPixel(random(LED_COUNT), 0, 0, 0);
        setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
       setPixel(random(LED_COUNT), 0, 0, 0);
        setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
       setPixel(random(LED_COUNT), 0, 0, 0);
        setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
       setPixel(random(LED_COUNT), 0, 0, 0);
        setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
       setPixel(random(LED_COUNT), 0, 0, 0);
        setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
       setPixel(random(LED_COUNT), 0, 0, 0);
        setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
       setPixel(random(LED_COUNT), 0, 0, 0);
        setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
       setPixel(random(LED_COUNT), 0, 0, 0);
        setPixel(random(LED_COUNT), 0, 0, 0);
         setPixel(random(LED_COUNT), 0, 0, 0);
      break;
    }
      }else{
        switch (bouncedirection)
    {
    case 1:
      Serial.print("IF idex-: ");
      Serial.println(idex);
      idex=MAX_LED_COUNT;
      bouncedirection=0;
      setPixel(random(LED_COUNT), 0, 0, 0);
      
      break;
    case 0:
      Serial.print("ELSE idex-: ");
      Serial.println(idex);
      idex=MIN_LED_COUNT;
      bouncedirection=1;
      setPixel(random(LED_COUNT), random(0, 255), random(0, 255), random(0, 255));
      break;
    }
        }
    FastLED.show();
    delay(delayValue); 
}

void FadeInOut()
{
  float r, g, b;
  
    int MIN_LED_COUNT = 10;
    int MAX_LED_COUNT = 255;
    if(idex>=MIN_LED_COUNT && idex<=MAX_LED_COUNT){
      switch (bouncedirection)
    {
    case 1:
      idex++;
      delay(10);
    r = (idex / 256.0) * mainColorR;
    g = (idex / 256.0) * mainColorG;
    b = (idex / 256.0) * mainColorB;
    updateColor(r, g, b);
    FastLED.show();
    if(idex==255){delay(430);}
      break;
    case 0:
      idex--;
      delay(10);
    r = (idex / 256.0) * mainColorR;
    g = (idex / 256.0) * mainColorG;
    b = (idex / 256.0) * mainColorB;
    updateColor(r, g, b);
    FastLED.show();
    if(idex==10){delay(390);}
      break;
    }
      }else{
        switch (bouncedirection)
    {
    case 1:
      idex=MAX_LED_COUNT;
      bouncedirection=0;
      break;
    case 0:
      idex=MIN_LED_COUNT;
      bouncedirection=1;
      break;
    }
        }
}

void RGBLoop()
{   
    if(ihue>=0 && ihue<=2){}else{ihue=0;}
    int MIN_LED_COUNT = 1;
    int MAX_LED_COUNT = 255;
    if(idex>=MIN_LED_COUNT && idex<=MAX_LED_COUNT){
      switch (bouncedirection)
    {
    case 1:
      Serial.print("BOUNCEDIR - 1 - ihue:");
      Serial.println(ihue);
      idex=idex+1;
      switch (ihue)
      {
      case 0:
      Serial.print("1 case 0 - ");
      Serial.print(" idex: ");
      Serial.print(idex);
      Serial.print(" ihue: ");
      Serial.println(ihue);
      updateColor(idex, 0, 0);
      if(idex==MAX_LED_COUNT){bouncedirection=0;}
        break;
      case 1:
      Serial.print("1 case 1 - ");
      Serial.print(" idex: ");
      Serial.print(idex);
      Serial.print(" ihue: ");
      Serial.println(ihue);
        updateColor(0, idex, 0);
        if(idex==MAX_LED_COUNT){bouncedirection=0;}
        break;
      case 2:
      Serial.print("1 case 2 - ");
      Serial.print(" idex: ");
      Serial.print(idex);
      Serial.print(" ihue: ");
      Serial.println(ihue);
        updateColor(0, 0, idex);
        if(idex==MAX_LED_COUNT){bouncedirection=0;}
        break;
      }
      FastLED.show();
      delay(3);
      break;
    case 0:
      Serial.print("BOUNCEDIR - 0 - ihue:");
      Serial.println(ihue);
      idex=idex-1;
      switch (ihue)
      {
      case 0:
        updateColor(idex, 0, 0);
         Serial.print("0 case 0 - ");
      Serial.print(" idex: ");
      Serial.print(idex);
      Serial.print(" ihue: ");
      Serial.println(ihue);
        if(idex==MIN_LED_COUNT){bouncedirection=1;ihue=1;}
        break;
      case 1:
       Serial.print("0 case 1 - ");
      Serial.print(" idex: ");
      Serial.print(idex);
      Serial.print(" ihue: ");
      Serial.println(ihue);
        updateColor(0, idex, 0);
        if(idex==MIN_LED_COUNT){bouncedirection=1;ihue=2;}
        break;
      case 2:
       Serial.print("0   case 2 - ");
      Serial.print(" idex: ");
      Serial.print(idex);
      Serial.print(" ihue: ");
      Serial.println(ihue);
        updateColor(0, 0, idex);
        if(idex==MIN_LED_COUNT){bouncedirection=1;ihue=0;}
        break;
      }
      FastLED.show();
      delay(3);
      break;
    }
      }else{
        switch (bouncedirection)
    {
    case 1:
      Serial.print("IF idex-: ");
      Serial.println(idex);
      idex=MAX_LED_COUNT;
      bouncedirection=0;
      break;
    case 0:
      Serial.print("ELSE idex-: ");
      Serial.println(idex);
      idex=MIN_LED_COUNT;
      bouncedirection=1;
      //ihue++;
      break;
    }
        }
    FastLED.show();
    delay(delayValue);
}

void colorWipe(int SpeedDelay)
{
  int COLOR_WIPE_LED_COUNT = 0;
  if (idex + 2 > LED_COUNT)
  {
    idex = 0;
    switch (bouncedirection)
    {
    case 1:
      bouncedirection = 0;
      break;
    case 0:
      bouncedirection = 1;
      break;
    }
  }
  COLOR_WIPE_LED_COUNT = idex + 1;
  if (bouncedirection == 0)
  {
    for (; idex <= COLOR_WIPE_LED_COUNT; idex++)
    {
      setPixel(idex, mainColorR, mainColorG, mainColorB);
      FastLED.show();
      Serial.print("idex: ");
      Serial.println(idex);
        delay(delayValue);
    }
  }
  else
  {
    for (; idex <= COLOR_WIPE_LED_COUNT; idex++)
    {
      setPixel(idex, 0, 0, 0);
      FastLED.show();
      Serial.print("idex: ");
      Serial.println(idex);
        delay(delayValue);
    }
  }
}

void colorWipe2(int SpeedDelay)
{
  int COLOR_WIPE_LED_COUNT = 0;
  if (idex + 2 > LED_COUNT)
  {
    idex = 0;
    switch (bouncedirection)
    {
    case 1:
      bouncedirection = 0;
      break;
    case 0:
      bouncedirection = 1;
      break;
    }
  }
  COLOR_WIPE_LED_COUNT = idex + 1;
  
  int size = LED_COUNT / 4;
  for (; idex <= COLOR_WIPE_LED_COUNT; idex++)
  {
    setPixel(idex, mainColorR, mainColorG, mainColorB);
    if (idex - size >= 0)
    {
      setPixel(idex - size, 0, 0, 0);
    }
    else
    {
      setPixel(LED_COUNT - size + idex, 0, 0, 0);
    }
    FastLED.show();
    delay(delayValue);
  }
}
void longColorWipe(int SpeedDelay)
{
  int COLOR_WIPE_LED_COUNT = 0;
  if (idex + 2 > LED_COUNT)
  {
    idex = 0;
    switch (bouncedirection)
    {
    case 1:
      bouncedirection = 0;
      break;
    case 0:
      bouncedirection = 1;
      break;
    }
  }
  COLOR_WIPE_LED_COUNT = idex + 1;
  
  int size = LED_COUNT / 2;
  for (; idex <= COLOR_WIPE_LED_COUNT; idex++)
  {
    setPixel(idex, mainColorR, mainColorG, mainColorB);
    if (idex - size >= 0)
    {
      setPixel(idex - size, 0, 0, 0);
    }
    else
    {
      setPixel(LED_COUNT - size + idex, 0, 0, 0);
    }
    FastLED.show();
    delay(delayValue);
  }
}
