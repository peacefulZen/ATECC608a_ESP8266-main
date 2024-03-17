#include <Arduino.h>
extern "C"
{
#include "cryptoauthlib.h"
}
#include "aes_cbc.h"
#include <EEPROM.h>
// Key Slot number
// uint8_t KEY_SLOT = (uint8_t)9;
uint8_t KEY_SLOT = (uint8_t)9;

ATCAIfaceCfg cfg;
ATCA_STATUS status;
//uint8_t iv[IV_LENGTH_CBC] = "#k2~>GA8QRl9YJ0";
uint addr = 80;

void saveData()
{
    ATCA_STATUS status = atcab_init(&cfg);
    if (status != ATCA_SUCCESS)
    {
        Serial.println(F("atcab_init() failed : Code -> 0x"));
        Serial.println(status, HEX);
    }
    // uint addr = 0; // Address for EEPROM
    uint8_t plaintext[32] = "bradford_Family_Comcast";

    uint8_t cypherdata[sizeof(plaintext)];
    // uint8_t decryptdata[sizeof(plaintext)];

    Serial.println("");
    Serial.println("Data size of buffers is:");
    Serial.println(sizeof(plaintext));

    Serial.println("");
    Serial.println("Printing plaintext Hex data.");
    Serial.println("");

    /*** Testing */
    for (size_t i = 0; i < sizeof(plaintext); i++)
    {
        // Serial.print((char)FP_cypherdata[i]);
        Serial.print(plaintext[i], HEX);
        Serial.print(" ");
    }

    Serial.println("");
    Serial.println("Encrypting 'bradford_Family_Comcast' in key!");
    Serial.println("");
    status = aes_cbc_encrypt(&cfg, plaintext, sizeof(plaintext), cypherdata, KEY_SLOT);
    if (status == ATCA_SUCCESS)
    {
        Serial.println("");
        Serial.println("Encryption successful.");
        Serial.println("");
        Serial.println("");
        Serial.println("Printing encrypted data before save to EEPROM.");
        Serial.println("");
        /*** Testing */
        for (size_t i = 0; i < sizeof(cypherdata); i++)
        {
            // Serial.print((char)cypherdata[i]);
            Serial.print(cypherdata[i], HEX);
            Serial.print(" ");
        }
        // EEPROM.begin (512); // commit 512 bytes of ESP8266 flash (for "EEPROM" emulation)
        Serial.println("");
        Serial.println("Saving encrypted data.");
        Serial.println("");
        EEPROM.put(addr, cypherdata);
        // EEPROM.put(addr, plaintext);
        EEPROM.commit();
        Serial.println("");
        Serial.println("Encryption data saved successfully.");
        Serial.println("");
        // Serial.println("Fetching encrypted data from saved location.");
        // Serial.println("");
        // // uint8_t FP_cypherdata[sizeof(plaintext)]; // Calling it something else to make sure we are in fact pulling it from EEPROM
        // uint8_t FP_cypherdata[32]; // Calling it something else to make sure we are in fact pulling it from EEPROM
        // EEPROM.get(addr, FP_cypherdata);
        // Serial.println("");
        // Serial.println("Decrypting fetched data.");
        // Serial.println("");
        // status = aes_cbc_decrypt(&cfg, FP_cypherdata, sizeof(FP_cypherdata), iv, decryptdata, KEY_SLOT);
        // if (status == ATCA_SUCCESS)
        // {
        //     Serial.println("");
        //     Serial.println("Decryption successful.");
        //     Serial.println("");
        //     Serial.print("Decrypted text is : ");
        //     for (size_t i = 0; i < sizeof(decryptdata); i++)
        //     {
        //         Serial.print((char)decryptdata[i]);
        //     }
        //     Serial.println("");
        // }
        // else
        // {
        //     // See file atca_status.h for the code Error
        //     Serial.print(F("Impossible do the decryption | Code Error 0x"));
        //     Serial.println(status, HEX);
        //     return;
        // }
    }
    else
    {
        // See file atca_status.h for the code Error
        Serial.print(F("Impossible do the encryption | Code Error 0x"));
        Serial.println(status, HEX);
        return;
    }
    delay(1000);
}

void fetchData()
{
    ATCA_STATUS status1 = atcab_init(&cfg);
    if (status1 != ATCA_SUCCESS)
    {
        Serial.println(F("atcab_init() failed : Code -> 0x"));
        Serial.println(status1, HEX);
    }

    Serial.println("Fetching encrypted data from saved location.");
    Serial.println("");
    // uint8_t FP_cypherdata[sizeof(plaintext)]; // Calling it something else to make sure we are in fact pulling it from EEPROM
    uint8_t FP_cypherdata[32]; // Calling it something else to make sure we are in fact pulling it from EEPROM
    uint8_t decryptdata[32];
    EEPROM.get(addr, FP_cypherdata);
    Serial.println("");
    Serial.println("Printing encrypted data fetched from EEPROM before decryption.");
    Serial.println("");

    /*** Testing */
    for (size_t i = 0; i < sizeof(FP_cypherdata); i++)
    {
        // Serial.print((char)FP_cypherdata[i]);
        Serial.print(FP_cypherdata[i], HEX);
        Serial.print(" ");
    }

    Serial.println("");
    Serial.println("Decrypting fetched data.");
    Serial.println("");
    status1 = aes_cbc_decrypt(&cfg, FP_cypherdata, sizeof(FP_cypherdata), decryptdata, KEY_SLOT);
    if (status1 == ATCA_SUCCESS)
    {
        Serial.println("");
        Serial.println("Decryption successful.");
        Serial.println("");
        Serial.println("Printing decrypted Hex data.");
        Serial.println("");

        /*** Testing */
        for (size_t i = 0; i < sizeof(decryptdata); i++)
        {
            // Serial.print((char)FP_cypherdata[i]);
            Serial.print(decryptdata[i], HEX);
            Serial.print(" ");
        }

        Serial.println("");
        Serial.print("Decrypted String text is : ");
        for (size_t i = 0; i < sizeof(decryptdata); i++)
        {
            Serial.print((char)decryptdata[i]);
        }
        Serial.println("");
    }
    else
    {
        // See file atca_status.h for the code Error
        Serial.print(F("Impossible do the decryption | Code Error 0x"));
        Serial.println(status1, HEX);
        return;
    }
}

void deleteAllData()
{
    Serial.println();
    Serial.println();
    Serial.println("Deleting all data from EERPOM.");
    Serial.println();
    Serial.print("EEPROM.length() == ");
    Serial.print(EEPROM.length());
    Serial.println();
    for (int i = 0; i < EEPROM.length(); i++)
    {
        // EEPROM.write(i, 0);
        EEPROM.put(i, 0);
    }
    EEPROM.commit();
    delay(500);
}

void setup()
{
    Serial.begin(74880);
    EEPROM.begin(512);
    // Init the constuctor for the library
    cfg.iface_type = ATCA_I2C_IFACE;  // Type of communication -> I2C mode
    cfg.devtype = ATECC608A;          // Type of chip
    cfg.atcai2c.slave_address = 0X30; // I2C address of the Adafruit Breakout Board
    cfg.atcai2c.bus = 1;
    cfg.atcai2c.baud = 100000;
    cfg.wake_delay = 1500; // Delay of wake up (1500 ms)
    cfg.rx_retries = 20;
    atca_trace_config(stdout);
    // deleteAllData();
    // saveData();
    // delay(1000);
    fetchData();
}

void loop()
{
}