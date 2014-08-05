#define SOLENOID_PIN 3
#define BRAKE_PIN 9
#define DIRECTION_PIN 12
#define RING_DELAY 200
#define RING_GAP 1000

#define SERIAL_BUFSIZE 200
#define COMP_STR(str1, str2) (strcmp(str1, str2) == 0)

char serial_buffer[SERIAL_BUFSIZE];
char n_buffer[SERIAL_BUFSIZE];

void ring_bell(void)
{
  digitalWrite(DIRECTION_PIN, HIGH);
  delay(RING_DELAY);
  digitalWrite(DIRECTION_PIN, LOW);
  delay(RING_DELAY);
}

void ring_bell_n(int ntimes)
{
  int i;
  for(i=0; i<ntimes; i++) {
    ring_bell();
    delay(RING_GAP);
  }
}

void setup()
{
  Serial.begin(9600);
  digitalWrite(SOLENOID_PIN, HIGH);
  digitalWrite(BRAKE_PIN, LOW);
  digitalWrite(DIRECTION_PIN, LOW);
}

void loop()
{
  if( Serial.available() ) {
    Serial.readBytesUntil('\n', serial_buffer, SERIAL_BUFSIZE - 1);
    if( COMP_STR(serial_buffer, "RING") ) {
      Serial.println("Bell ring received!");
      ring_bell();
    }
    else if( strstr(serial_buffer, "RINGN") ) {
      strncpy(n_buffer, serial_buffer + 6, SERIAL_BUFSIZE);
      Serial.print("Bell ring n received: ");
      Serial.print(n_buffer);
      Serial.println(" times");
      ring_bell_n(atoi(n_buffer));
    } 
    else if( COMP_STR(serial_buffer, "UPTIME") ) {
      Serial.println(millis() / 1000 / 60);
    }
    else {
      Serial.println("Unrecognised command.");
    }
    memset(serial_buffer, '\0', SERIAL_BUFSIZE);
  }  
}
