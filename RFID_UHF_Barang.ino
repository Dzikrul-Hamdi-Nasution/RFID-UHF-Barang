#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#define header 0xBB   // mendefinisikan frame pembuka pesan
#define last 0x7E     // Mendefinisikan frame penutup pesan
#define indexCode 2
#define en 4
byte type[2] = {2, 1};
byte code[indexCode] = {0x22, 0xFF};

bool valid = true;
bool started = true;
bool pass = false;
String respond;

uint8_t singleRead[7] = {0xBB, 0x00,
                         0x22, 0x00, 0x00, 0x22, 0x7E
                        };
uint8_t multiRead[10] = {0xBB, 0x00,
                         0x27, 0x00, 0x03, 0x22, 0x00,
                         0x10, 0x5c, 0x7E
                        };
String dataIn;
String dt[10];
int i;
boolean parsing = false;


String kode_rfid;
#define FIREBASE_HOST "rfid-uhf-logger-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "9LB1MPKCvBrWGh3DDbmieh62BmreuDos694C3v6c"
#define WIFI_SSID "Server Project"
#define WIFI_PASSWORD "Salamproject2022"

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);


  for (int i = 0; i < 7 ; i++) {
    Serial.write(singleRead[i]);
    delay(50);
  }
  Serial.println();
  while (WiFi.status() != WL_CONNECTED)
  {

    delay(300);
  }
  Serial.println("Tes RFID UHF");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("status", "Offline");

}

void loop() {
  //filter data masuk

  scanning_rfid();

  String mode_operasi = Firebase.getString("mode/nilai");
  if (mode_operasi == "input") {

    int panjang_data = respond.length();
    String sub_S = respond.substring(9, 36);
    if (panjang_data > 20 and panjang_data < 45 ) {
      Firebase.setString("scan_rfid", sub_S);
      // Firebase.setString("List/" + sub_S + "/nilai", sub_S);
    }
  }



  if (mode_operasi == "_esoutput") {

    int panjang_data = respond.length();
    String sub_S = respond.substring(9, 36);
    if (panjang_data > 20 and panjang_data < 45 ) {
      String inventaris = Firebase.getString("List/" + sub_S + "/jenis");
      if (inventaris == NULL) {
        //tidak melakukan apa apa
      }
      else {
        String jumlah_inventaris = Firebase.getString("ID/" + inventaris + "/jumlah");
        jumlah_inventaris = jumlah_inventaris.toInt() - 1;
        Firebase.setString("ID/" + inventaris + "/jumlah", jumlah_inventaris);
        while (panjang_data < 20) {
          scanning_rfid();
          int panjang_data = respond.length();


        }

      }
    }

  }


  //Serial.println(respond);






}

void scanning_rfid() {
  int crc;
  uint16_t paramL = 0;
  uint8_t data = 0;

  for (int i = 0; i < 7 ; i++) {
    Serial.write(singleRead[i]);
    delay(50);
  }
  Serial.println();

  while (Serial.available()) {
    data = Serial.read();
    if (data == 0xBB) {
      pass = data == 0xBB ? true : false;
      data = Serial.read();
      crc = data;
      pass = matchType(data);
      data = Serial.read();
      crc += data;
      pass = matchCode(data);
      if (pass) {
        paramL = Serial.read();
        crc += paramL;
        paramL = paramL << 8;
        paramL += Serial.read();
        crc += paramL;
        respond += String(paramL, HEX) ;
        for (int i = 0; i < paramL; i++) {
          data = Serial.read();
          respond += String(data, HEX) ;
          crc += data;
        }
        crc &= 0xFF;
        data = Serial.read();
        if (crc == data) {
          respond += String(crc, HEX) ;
          data = Serial.read();
          respond += String(data, HEX);
        }

      }
    }
  }
}



bool matchType(uint8_t data) {
  for (int i = 0; i < 2; i++) {
    if (data == type[i]) {
      respond = "bb";
      respond += String(data, HEX) ;
      return true;
    }
  }
  return false;
}

bool matchCode(uint8_t data) {
  for (int i = 0; i < indexCode; i++) {
    if (data == code[i]) {
      respond += String(code[i], HEX) ;
      return true;
    }
  }
  return false;
}
