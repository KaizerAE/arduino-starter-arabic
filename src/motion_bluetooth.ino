/*
  مشروع: نظام كشف حركة بمستشعر PIR مع إرسال إشعار عبر البلوتوث
  الوصف: عند اكتشاف الحركة بواسطة مستشعر PIR، يرسل الأردوينو رسالة عبر البلوتوث
         (HC-05/HC-06) لتطبيق الهاتف، مع طباعة رسائل حالة الاتصال.
  الفكرة: مثال عملي لمراقبة المنزل الذكي.

  التوصيلات المقترحة:
  - مستشعر PIR (مثال HC-SR501):
      VCC -> 5V
      GND -> GND
      OUT -> D2 (مدخل رقمي مع خاصية المقاطعة إن رغبت)
  - وحدة بلوتوث HC-05/HC-06:
      VCC -> 5V
      GND -> GND
      TXD -> RX الأردوينو
      RXD -> TX الأردوينو (يفضل مقسم جهد إلى 3.3V لحماية الوحدة)

  الملاحظات:
  - إن كنت تستخدم Arduino Uno: استخدم Serial الافتراضي للتواصل مع البلوتوث،
    مع العلم أنه يُشغل نفس منفذ USB التسلسلي. لبرمجة اللوحة افصل الـ RX/TX
    عن البلوتوث مؤقتًا لتفادي التعارض أثناء الرفع.
  - يمكن استخدام SoftwareSerial على لوحات محددة إذا أردت إبقاء Serial
    للـ USB فقط.
*/

// اختر أحد الخيارين حسب احتياجك:
//#include <SoftwareSerial.h>
//SoftwareSerial BT(10, 11); // RX, TX (وصل TX البلوتوث إلى 10، وRX البلوتوث إلى 11 عبر مقسم جهد)

const int PIR_PIN = 2;         // طرف خرج مستشعر PIR
const unsigned long DEBOUNCE_MS = 200; // فلترة بسيطة لمنع الاهتزازات
volatile bool motionFlag = false;      // يُضبط عند حدوث حركة
unsigned long lastTrigger = 0;

void IRAM_ATTR onMotion() {  // تُستدعى عند تغير الإشارة (إن كانت اللوحة تدعم المقاطعات بهذه الصيغة)
  unsigned long now = millis();
  if (now - lastTrigger > DEBOUNCE_MS) {
    motionFlag = true;
    lastTrigger = now;
  }
}

void setup() {
  pinMode(PIR_PIN, INPUT);

  // بدء الاتصال التسلسلي مع البلوتوث عبر Serial الافتراضي بسرعة 9600 (شائعة لوحدات HC-05/06)
  Serial.begin(9600);
  delay(500);

  // إن استخدمت SoftwareSerial، افعل بدلًا من ذلك:
  // BT.begin(9600);

  // طباعة رسالة ترحيب وتعريف المشروع
  Serial.println("[نظام مراقبة الحركة] تم التشغيل ✅");
  Serial.println("يرجى فتح تطبيق البلوتوث على الهاتف والاتصال بالوحدة.");
  Serial.println("عند الاتصال ستظهر رسالة ترحيبية، وسيتم إرسال إشعار عند اكتشاف الحركة.");

  // تفعيل مقاطعة على طرف D2 (Uno: interrupt 0) للكشف السريع عن الحركة
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), onMotion, RISING);
}

void loop() {
  // كشف حالة الاتصال: عند فتح اتصال تسلسلي من تطبيق الهاتف، يرسل Arduino رسالة ترحيب
  if (Serial && Serial.availableForWrite()) {
    static bool greeted = false;
    if (!greeted) {
      Serial.println("[Bluetooth] تم الاتصال بالتطبيق 👋");
      Serial.println("أرسل أي حرف لاختبار التواصل.");
      greeted = true;
    }
  }

  // استقبال أي بيانات قادمة من التطبيق (اختياري)
  if (Serial.available()) {
    char c = Serial.read();
    Serial.print("استقبلت: ");
    Serial.println(c);
  }

  // عند اكتشاف حركة عبر المقاطعة
  if (motionFlag) {
    motionFlag = false;

    // قراءة حالة المستشعر للتأكد
    int state = digitalRead(PIR_PIN);
    if (state == HIGH) {
      // إرسال إشعار عبر البلوتوث
      Serial.println("[إنذار حركة] تم اكتشاف حركة! 🚨");
      Serial.println("نصيحة: تحقق من المكان أو ابدأ تسجيل فيديو إن توفر.");
    }
  }
}
