#include <DHT.h>
#include <Wire.h>
#include <MPU6050.h>
#include <VirtualWire.h>

#define DHTPIN 2       // Pin where DHT11 is connected
#define DHTTYPE DHT11  // Type of DHT sensor

DHT dht(DHTPIN, DHTTYPE);

MPU6050 mpu;

void setup() {
  // Initialize DHT sensor
  dht.begin();

  // Initialize MPU6050 sensor
  Wire.begin();
  mpu.initialize();

  // Initialize VirtualWire library
  vw_setup(2000);  // Bits per second
  vw_set_tx_pin(12);  // Transmit pin
  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
  delay(2000); // DHT11 sensor reading interval

  // Read DHT11 data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if DHT readings are valid
  if (!isnan(humidity) && !isnan(temperature)) {
    // Create a DHT data packet
    char dhtMsg[30];
    snprintf(dhtMsg, sizeof(dhtMsg), "Temp: %.2f C, Humidity: %.2f%%", temperature, humidity);

    // Send the DHT data packet
    vw_send((uint8_t *)dhtMsg, strlen(dhtMsg));
    vw_wait_tx(); // Wait for transmission to complete

    // Print the sent DHT data to the serial monitor for debugging
    Serial.println("DHT Data sent: " + String(dhtMsg));
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  // Read MPU6050 data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Create an MPU6050 data packet
  char mpuMsg[30];
  snprintf(mpuMsg, sizeof(mpuMsg), "AccX: %d, AccY: %d, AccZ: %d", ax, ay, az);

  // Send the MPU6050 data packet
  vw_send((uint8_t *)mpuMsg, strlen(mpuMsg));
  vw_wait_tx(); // Wait for transmission to complete

  // Print the sent MPU6050 data to the serial monitor for debugging
  Serial.println("MPU6050 Data sent: " + String(mpuMsg));

  // Add a delay or other logic as needed for your application
  delay(500); // Delay for 0.5 seconds before the next reading
}
