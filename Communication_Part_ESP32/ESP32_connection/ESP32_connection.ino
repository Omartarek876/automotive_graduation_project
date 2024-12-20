#define RXD2 16 // GPIO16 للـ RX الخاص بـ UART2
#define TXD2 17 // GPIO17 للـ TX الخاص بـ UART2

void setup() {
  // تهيئة UART0 (USB Serial) للتواصل مع Serial Monitor
  Serial.begin(115200);

  // تهيئة UART2 للتواصل مع Tiva C
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  
  byte message = '5';
  // 1. إرسال البيانات إلى Tiva C
  Serial2.println(message); // إرسال الرسالة عبر UART2
  Serial.println("Message sent to Tiva C" + message); // تأكيد الإرسال على Serial Monitor

  // 2. استقبال البيانات من Tiva C وعرضها
  if (Serial2.available()) {
    // Read one byte from UART2
    byte receivedByte = Serial2.read(); 
    Serial.print("Byte received from Tiva C: ");
    Serial.println(receivedByte); // Display the byte in hexadecimal format
  } else {
    Serial.println("No data available on UART2."); // Debugging log
  }
  delay(2000); // تأخير قبل الدورة التالية
}