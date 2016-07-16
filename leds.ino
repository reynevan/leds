#define COLORS_NUM 5


struct Color {
  int red;
  int green;
  int blue;
};


class Led{
  private:
  int redPin;
  int greenPin;
  int bluePin;

  int timeLow, timeHigh;
  int timeSparkLow, timeSparkHigh;
  int redLow, redHigh;
  int greenLow, greenHigh;
  int blueLow, blueHigh;
  int sparkChance;
  int steps;

  int colorIndex = 0;
  struct Color colors[COLORS_NUM] = {
      {255,255,0},
      {120, 0, 255},
      {0,255,0},
      {0,0,255},
      {250,40,0}
    };
  
  Color low, high, sparksLow, sparksHigh;
  
  bool changing;
  int aStep;
  Color start;
  Color target;
  int delayTime;

  void initChanging();
  void spark();
  
 public:
 Led(int redPin, int greenPin, int bluePin): redPin(redPin), greenPin(greenPin), bluePin(bluePin){
    red = 0;
    green = 0;
    blue = 0;
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);

    changing = false;
    aStep = 0;
    sparksLow.red = 0;
    sparksLow.green = 0;
    sparksLow.blue = 0;
    
    sparksHigh.red = 0;
    sparksHigh.green = 0;
    sparksHigh.blue = 0;

    dynamicMode = false;
    
 }
  void runChanger();
  void setColor(int, int, int);
  void render();
  
  void setFire();
  void setOcean();
  void setForest();

  void setLow(int, int, int);
  void setHigh(int, int, int);
  void setSparksLow(int, int, int);
  void setSparksHigh(int, int, int);
 
  void setTime(int tl, int th){ timeLow = tl; timeHigh = th; }
  void setSparkTime(int tl, int th){ timeSparkLow = tl; timeSparkHigh = th; }
  void setSparks(int chance){ sparkChance = chance; }
  void setSteps(int iSteps){ steps = iSteps; }

  void initEffect();

  void nextColor();
  void dupa();

  bool dynamicMode;
  int blue;
  int green;
  int red;
};
void handleClapping();
void initLeds();

Led led (3, 5, 6);
float temp;

void setup() {
  Serial.begin(9600);
  led.initEffect();
  randomSeed(analogRead(0)); 
}

int n = 9999;

void loop() {
  int red = 0;
  int green = 0;
  int blue = 0;
  if (Serial.available()){
    red = Serial.parseInt(); 
    green = Serial.parseInt(); 
    blue = Serial.parseInt(); 
    if (Serial.read() == '\n') {   
      if (red == -1 && green == 0 && blue == 0){
        led.setFire();  
      }   
      else if (red == 0 && green == -1 && blue == 0){
        led.setOcean(); 
      } else if(red == 0 && green == 0 && blue == -1){
        led.setForest();
      } else if(red == -1 && green == -1 && blue == 0){
        led.dynamicMode = false;
        led.initEffect();
      } else {
        led.setColor(red, green, blue);  
        led.render();
      }
    delay(100);
    }     
  }
  /*if (n++ > 10000){
    temp = (5.0 * analogRead(A0) * 100.0) / 1024;
    //Serial.println(temp);
    delay(10);
    n ^= n;
  }*/
  if (led.dynamicMode){
    led.runChanger();
  }
  //handleClapping(&led);
}

void handleClapping(Led* led){
  static unsigned long timestamp = millis();
  unsigned long timediff = 0;
  static int clapped = 0;  
  static int threshold = 45;
  static int timeThreshold = 500;
  
  
  if (clapped == 2 && millis() - timestamp > timeThreshold) {
    led->nextColor(); 
    led->render();
    clapped = 0;
  }
  if (clapped > 0){
    if (millis() - timestamp > timeThreshold){  
      clapped = 0;  
    }
  }
    
  int sound = analogRead(A4);
  //Serial.println(sound);
 
  
  if (sound > threshold){
    Serial.println(sound);
    timestamp = millis();
      
    if (clapped > 0){
      timediff = millis() - timestamp;
      //Serial.println(timediff);
      if (timediff < timeThreshold){        
        if (clapped == 2){
          clapped = 0; 
          led->setColor(0,0,0);
          led->render();
          //Serial.println("dupa");
        } else {
          clapped = 2;  
        }
      } else {
        clapped = 1;  
      }
    } else {
      clapped = 1;
    }
    delay(100);
  } 
  
}

void Led::setColor(int r, int g, int b){
  red = r;
  green = g;
  blue = b;

  dynamicMode = false;
}

void Led::render(){  
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void initLeds(){
  int red, blue, green;
  float deg = 0;
  for (int i = 0; i < 150; i ++){
    deg += PI/75;
    red = (float)255*sin(deg);
    green = (float)255*sin(deg+(2*PI/3));
    blue = (float)255*sin(deg-(2*PI/3));
    red = red > 0 ? red : 0;
    green = green > 0 ? green : 0;
    blue = blue > 0 ? blue : 0;
    led.setColor(red, green , blue);
    led.render();
    delay(5);
  }
}

void Led::setLow(int r, int g, int b){
  low.red = r;
  low.green = g;
  low.blue = b;
}
void Led::setHigh(int r, int g, int b){
  high.red = r;
  high.green = g;
  high.blue = b;
}
void Led::setSparksLow(int r, int g, int b){
  sparksLow.red = r;
  sparksLow.green = g;
  sparksLow.blue = b;
}
void Led::setSparksHigh(int r, int g, int b){
  sparksHigh.red = r;
  sparksHigh.green = g;
  sparksHigh.blue = b;
}

void Led::setFire() {
  setLow(170, 10, 0);
  setHigh(255, 40, 0);
  setSparksLow(200, 40, 0);
  setSparksHigh(255, 50, 0);
  setTime(100, 500);
  setSparkTime(10, 100);
  setSteps(20);
  setSparks(85);
  dynamicMode = true;
}

void Led::setOcean(){  
  setLow(0, 30, 150);
  setHigh(0, 100, 255);
  setTime(500, 3000);
  setSparkTime(0, 0);
  setSteps(50);
  setSparks(0);
  dynamicMode = true;
}

void Led::setForest(){
  setLow(0, 160, 0);
  setHigh(150, 255, 10);
  setTime(50, 2000);
  setSparkTime(0, 0);
  setSteps(50);
  setSparks(0);
  dynamicMode = true;
  
}

void Led::initChanging(){
  changing = true;
  aStep = 0;
  target.red = random(low.red, high.red);
  target.green = random(low.green, high.green);
  target.blue = random(low.blue, high.blue);
  start.red = red;
  start.green = green;
  start.blue = blue;
  delayTime= random(timeLow, timeHigh);
}

void Led::spark(){
    red = random(sparksLow.red, sparksHigh.red);
    green = random(sparksLow.green, sparksHigh.green);
    blue = random(sparksLow.blue, sparksHigh.blue);
    render();
    delay(random(timeSparkLow, timeSparkHigh));
}

void Led::runChanger(){
  if (changing){
    if (aStep <= steps){
      red = int(start.red + ((target.red - start.red) / steps) * aStep);
      green = int(start.green + ((target.green - start.green) / steps) * aStep);
      blue = int(start.blue + ((target.blue - start.blue) / steps) * aStep);
      aStep++;
      render();
      
      delay(delayTime / steps);
    } else {
      changing = false;
    }
  } else {
    if (random(0, 100) > sparkChance){
      initChanging();
    } else {
      spark();
    }
  }
}

void Led::initEffect(){
  float deg = 0;
  for (int i = 0; i < 150; i ++){
    deg += PI/75;
    red = (float)255*sin(deg);
    green = (float)255*sin(deg+(2*PI/3));
    blue = (float)255*sin(deg-(2*PI/3));
    red = red > 0 ? red : 0;
    green = green > 0 ? green : 0;
    blue = blue > 0 ? blue : 0;
    render();
    delay(8);
  }
  red = 0;
  green = 0;
  blue = 0;
  render();
}

void Led::nextColor(){
  
  Color c = colors[colorIndex];
  
  if (++colorIndex >= COLORS_NUM){
    colorIndex = 0;  
  }
  
  setColor(c.red, c.blue, c.green);
}

