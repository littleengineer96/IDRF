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

    rfid.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    rfid.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD
}

#endif