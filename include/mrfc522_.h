#ifndef MRFC522_H_LIB
#define MRFC522_H_LIB

#include "prototypes.h"

void MRFC522_get_id();
// void MRFC522_setup();
void MRFC522_open(unsigned long time);

MFRC522 rfid(PIN_CS_MRFC522, PIN_RST_MRFC522);

void MRFC522_setup()
{
    SPI.begin();
    rfid.PCD_Init();
    // MRFC522_get_id();
    // rfid.PCD_Reset();
    
}

void MRFC522_get_id()
{
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) // VERIFICA SE O CARTÃO PRESENTE NO LEITOR É DIFERENTE DO ÚLTIMO CARTÃO LIDO. CASO NÃO SEJA, FAZ
        return;                                                       // RETORNA PARA LER NOVAMENTE

    /***INICIO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA***/
    String strID = "";
    for (byte i = 0; i < 4; i++)
    {
        strID +=
            (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
            String(rfid.uid.uidByte[i], HEX) +
            (i != 3 ? ":" : "");
    }
    strID.toUpperCase();
    /***FIM DO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA***/

    Serial.print("Identificador (UID) da tag: "); // IMPRIME O TEXTO NA SERIAL
    Serial.println(strID);                        // IMPRIME NA SERIAL O UID DA TAG RFID

    // String UID_CARD = (String)UID_TAG_CARD;
    // String UID_CHAVEIRO

    if (strID.compareTo(UID_TAG_CARD) == 0)
    {
        Serial.println("Acesso liberado: card ok");
        MRFC522_open(2000);
    }

    if (strID.compareTo(UID_TAG_CHAVEIRO) == 0)
    {
        Serial.println("Acesso liberado: chaveiro ok\n");
        MRFC522_open(2000);
    }

    rfid.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    rfid.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD
}

void MRFC522_open(unsigned long time)
{
    // ledcWrite(0, 30);
    // delay(200);
    
    // ledcWrite(0, 50);
    // delay(500);

    // ledcWrite(0, 70);
    // delay(200);

    // ledcWrite(0, 100);
    // time = 2000;
    // delay(time);
    // ledcWrite(0, 0);

    // digitalWrite(PIN_SOLENOIDE, HIGH);
    // delay(100);
    // digitalWrite(PIN_SOLENOIDE, LOW);
    // delay(10);
    digitalWrite(PIN_SOLENOIDE, HIGH);
    time = 3000;
    delay(time);
    digitalWrite(PIN_SOLENOIDE, LOW);
}
#endif