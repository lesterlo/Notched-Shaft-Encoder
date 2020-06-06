// Notched Shaft Encoder cpp file (NSEncoder.cpp)
// Copyright (c) 2020 Lester Lo 
// MIT License

#include "NSEncoder.h"

NSEncoder::NSEncoder(   uint8_t pinA, 
                        uint8_t pinB, 
                        uint8_t stepsPerNotch
                    )
        :   Encoder(pinA, pinB),
            _stepsPerNotch(stepsPerNotch)
{}

NSEncoder::~NSEncoder()
{

}

bool NSEncoder::get_Position(int16_t &return_value)
{
    bool updated_flag = false;
    int16_t enc_cur_pulse = this->read(); //Read the encoder reading

    //1. If the Encoder Value Changed 
    if(enc_cur_pulse != enc_old_pulse)
    {
        int16_t convert_step = enc_cur_pulse / _stepsPerNotch; //Convert the pulse to step

        //1b. If the Coverted step value is also change
        if(enc_old_step != convert_step)
        {
            return_value = convert_step; //Update the return value
            updated_flag = true; //Set the flag that the step value is updated.
            enc_old_step = convert_step;  //Memorize the current step count
        }
        enc_old_pulse = enc_cur_pulse;//Memorize the current pulse count
    }
   
    return updated_flag; //Return the flag
}

int16_t NSEncoder::get_diffPosition()
{
    int16_t last_step = enc_old_step;
    int16_t new_step = 0;

    if(this->get_Position(new_step))
        return new_step - last_step; //Calculate the step difference between current and last time
    else
        return 0;
}

void NSEncoder::reset()
{
    enc_old_step = 0; 
    enc_old_pulse = 0; 
    this->write(0);//Reset the enocder counter to 0
}