/*
 * مشروع: قياس درجة الحرارة وعرضها على شاشة LCD
 * الوصف: يقرأ هذا البرنامج درجة الحرارة من حساس LM35 ويعرضها على شاشة LCD 16x2
 * المكونات المطلوبة:
 * - Arduino Uno
 * - حساس حرارة LM35
 * - شاشة LCD 16x2
 * - مقاومة متغيرة 10K (Potentiometer) للتحكم بتباين الشاشة
 * - أسلاك توصيل
 * - Breadboard
 */

// استدعاء مكتبة LCD
#include <LiquidCrystal.h>

// تعريف أطراف توصيل شاشة LCD
// LiquidCrystal(rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// تعريف الطرف التناظري المتصل بحساس LM35
const int sensorPin = A0;

void setup() {
  // تهيئة شاشة LCD بعدد الأعمدة والصفوف
  lcd.begin(16, 2);
  
  // طباعة رسالة ترحيبية
  lcd.print("Temp Monitor");
  delay(2000);
  lcd.clear();
}

void loop() {
  // قراءة القيمة التناظرية من الحساس (0-1023)
  int sensorValue = analogRead(sensorPin);
  
  // تحويل القيمة إلى جهد كهربائي (0-5 فولت)
  float voltage = sensorValue * (5.0 / 1023.0);
  
  // تحويل الجهد إلى درجة حرارة (كل 10mV = 1 درجة مئوية)
  float temperatureC = voltage * 100;
  
  // تحويل درجة الحرارة من مئوية إلى فهرنهايت (اختياري)
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  
  // عرض درجة الحرارة بالدرجة المئوية على الشاشة
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatureC);
  lcd.print(" C");
  
  // عرض درجة الحرارة بالفهرنهايت على السطر الثاني
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatureF);
  lcd.print(" F");
  
  // الانتظار ثانية واحدة قبل القراءة التالية
  delay(1000);
}
