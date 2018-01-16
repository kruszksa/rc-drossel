/*
 * ToDos: 
 * - Kalibrierung des Nullwerts
 * - Drossel des Rückwärtsgangs verbessern
 */
int nullwert = 1490; // Neutralstellung der Fernbedienung
int ausgabewert = 1490; // Der Nullwert zur Initialisierung der Variable
int zwischenwert;
int drosselwert;
int rueckbegrenzer = 1235; // Rückfahrbegrenzer, Max. Wert zum Rückwertsfahren
int x = 5; // Speedwert Default auf langsam
int i;
boolean taster;
unsigned long len;

void setup()
{
  // Debug Purpose
  //Serial.begin(9600);
  // \Debug
  pinMode (9, OUTPUT);    // Datenleitung zum Regeler (meist weiß)
  pinMode (13, OUTPUT);   // LED auf dem Arduino
  pinMode (12, INPUT);    // Taster
  pinMode (3, INPUT);     // Datenleitung vom Empfänger (gelb/weiß)
}

void loop()
{
  // Prüfen ob der Taster gedrückt wurde
  taster = digitalRead(12);
  if (taster == HIGH)
  {
    // Speedwert um 1 erhöhen
    x++;
    switch (x) {
      case 1:
        // 1 mal blinken um Geschwindigkeitswert anzuzeigen, keine Drossel      
        led_blink(x);
        break;
      case 2:
        // 2 mal blinken um Geschwindigkeitswert anzuzeigen, Geschwindigkeit schnell
        led_blink(x);
        break;
      case 3:
      // 3 mal blinken um Geschwindigkeitswert anzuzeigen, Geschwindigkeit mittel
        led_blink(x);
        break;
      case 4:
      // 4 mal blinken um Geschwindigkeitswert anzuzeigen, Geschwindigkeit langsam
        led_blink(x);
        break;
      case 5:
      // 5 mal blinken um Geschwindigkeitswert anzuzeigen, Geschwindigkeit langsam
        led_blink(x);
        break;
      case 6:
        led_blink(1);
        break;
    }
    // Speedwert auf 1 setzen wenn 5 erreicht wurde
    if (x > 5) {
      x = 1;
    }
  }
  // Auslesen der Pulsweite des Senders ( kein Gas ca 1490 Mikrosekunden, Vollgas vorwärts ca 1990 Mikrosekunden, Vollgas Rückwerts ca. 990 Mikrosekunden )
  len = pulseIn(3, HIGH);
  // Debug
  //Serial.println(len);
  // \Debug
  drossel(x, nullwert, len);
}

// Funktionsdefinition um die LED blinken zu lassen
int led_blink(int x){
  for (i = 0; i < x; i++)
    {
      digitalWrite(13, HIGH); 
      delay (250);
      digitalWrite(13, LOW);
      delay (250);
    }
}

// Funktionsdefinition um die Drosselwerte zu berechnen und die gedrosselten Werte auszugeben
int drossel(int x,int nullwert,int len){
  // Rückwärts drosseln, auf die hälfte fest eingestellt
  if ( x > 1 && len < nullwert){
    if ( len > rueckbegrenzer){
      ausgabewert = len;
      ausgabe(ausgabewert);
    }
    else
    ausgabewert = rueckbegrenzer;
    ausgabe(ausgabewert);
  }
  // Vorwärts drosseln
  else if ( x > 1 && len > nullwert){
    zwischenwert = len - nullwert;
    drosselwert = zwischenwert / x;
    ausgabewert = nullwert + 9 + drosselwert;
    ausgabe(ausgabewert);  
  }
  // Keine Drossel wenn x = 1 oder len = nullwert
  else
    ausgabewert = len;
    ausgabe(ausgabewert);  
}

// Funktionsdefinition zum Ausgeben des PWM Signals
int ausgabe(int ausgabewert){
  // Debug
  //Serial.println(ausgabewert);
  // \Debug
  digitalWrite(9, HIGH); // pin9 auf HIGH setzen
  delayMicroseconds(ausgabewert); // puls in Mikrosekunden warten
  digitalWrite(9, LOW); //pin9 auf LOW setzen
  delay(20); // 20 ms warten 
}

