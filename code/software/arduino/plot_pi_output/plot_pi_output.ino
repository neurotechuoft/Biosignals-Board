

int input_pin = 2;
int led_pin = LED_BUILTIN;
int drdy_pin = 5;
int analog_pin = A5;

// Peak to peak voltage of incoming signal
const double VPP = 5.0;
const int bit_depth = 10;
int ADC_RESOLUTION;

int threshold = 1;

void setup() {
  
  // Make sure what ever you put here matches your Serial Monitor setting
  Serial.begin(9600);
  Serial.flush();
  pinMode(input_pin, INPUT);
  pinMode(analog_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(drdy_pin, OUTPUT);

  ADC_RESOLUTION = pow(2, bit_depth)-1;

} // end setup()

void loop() {

    digitalWrite(drdy_pin, HIGH);
    double ana_sample = analogRead(analog_pin); 
    ana_sample *= VPP / ADC_RESOLUTION;
    
    double dig_sample = digitalRead(input_pin);
    
    Serial.print(ana_sample); 
    Serial.print(" ");
    Serial.println(dig_sample);

    if (dig_sample) digitalWrite(led_pin, HIGH);
    else digitalWrite(led_pin, LOW);
    

} // end loop()



