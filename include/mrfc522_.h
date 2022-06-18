#ifndef MRFC522_H_LIB
#define MRFC522_H_LIB

#include "prototypes.h"

void MRFC522_get_id();
// void MRFC522_setup();

MFRC522 rfid(PIN_CS_MRFC522, PIN_RST_MRFC522);

void MRFC522_setup()
{

    SPI.begin();
    rfid.PCD_Init();
    MRFC522_get_id();
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
        digitalWrite(PIN_SOLENOIDE, HIGH);
        delay(2000);
        digitalWrite(PIN_SOLENOIDE, LOW);
    }

    if (strID.compareTo(UID_TAG_CHAVEIRO) == 0)
    {
        Serial.println("Acesso liberado: chaveiro ok\n");
        digitalWrite(PIN_SOLENOIDE, HIGH);
        delay(2000);
        digitalWrite(PIN_SOLENOIDE, LOW);
    }

    rfid.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    rfid.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD
}

#endif