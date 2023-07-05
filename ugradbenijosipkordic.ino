#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 16,2);

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

volatile bool connected = false;


void setup(void)
{
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight ();
  lcd.print ( "Prislonite" );
  lcd.setCursor(0, 1);
  lcd.print("karticu");

  Serial.begin(115200);
  Serial.println("*** Testing Module PN532 ***");
}

void loop(void)
{
  boolean success;
  // Buffer to store the UID
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  //uint8_t uid1[] = { 4, 5A, 63, 4A, 1E, 5D, 80 };
  //uint8_t uid2[] = { 4, 87, 2F, 4A, 1E, 5D, 80 };
  // UID size (4 or 7 bytes depending on card type)
  uint8_t uidLength;

  while (!connected) {
    connected = connect();
  }

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  // If the card is detected, print the UID
  if (success)
  {
    Serial.println("Card Detected");
    Serial.print("Size of UID: "); Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
    }
    Serial.println("");
    Serial.println("");
    if (uid[1] == 0x5A) {
    Serial.println("Josip Kordić");
    lcd.clear();         
    lcd.backlight();  
    lcd.print ( "Josip Kordic" );
    delay(3500);
    lcd.clear();  
    lcd.print ( "Prislonite" );
    lcd.setCursor(0, 1);
    lcd.print("karticu");  
    }
    else if (uid[1] == 0x87) {
    Serial.println("Marko Markovic");
    lcd.clear();         
    lcd.backlight();  
    lcd.print ( "Marko Markovic" );
    delay(3500);
    lcd.clear();  
    lcd.print ( "Prislonite" );
    lcd.setCursor(0, 1);
    lcd.print("karticu");  
    }
    else {
      lcd.clear();
      lcd.backlight();
      lcd.print("Neregistriran");
      delay(3500);
      lcd.clear();  
      lcd.print ( "Prislonite" );
      lcd.setCursor(0, 1);
      lcd.print("karticu"); 
    }

    delay(1000);
    connected = connect();
  }
  else
  {
    // PN532 probably timed out waiting for a card
    // Serial.println("Timed out waiting for a card");
  }
}

bool connect() {
  
  nfc.begin();

  // Connected, show version
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    Serial.println("PN53x card not found!");
    return false;
  }
  //port
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware version: "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for card (ISO14443A Mifare)...");
  Serial.println("");

  return true;
}