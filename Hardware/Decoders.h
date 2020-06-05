#ifndef DECODERS_H
#define DECODERS_H
#include "Hardwares.h"

/***
 * Class:Decoder(译码器74LS138)
 * Function：使能端有效时译码
 * Output：MicroCom::DE_y0~DE_y7低电平
 * 使能端有效：G1为高电平，G2a、G2b为低电平
 */

class Decoders : public Hardwares
{
private:
    Q_OBJECT
    int status;
    int buffer;
    Voltage pins[DECODER_PIN_NUM];
    Counter<Decoders> c;

public:
    Decoders(QString decoderName=nullptr);
    void setPinVoltage(MicroCom::Pins pin, Voltage value);
    void handlePinVolChanges(MicroCom::Pins pin, Voltage);
    Voltage getPinVoltage(MicroCom::Pins pin);
    static int howMany(){return Counter<Decoders>::howMany();}
};

#endif // DECODERS_H
