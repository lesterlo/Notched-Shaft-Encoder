// Notched Shaft Encoder header file (NSEncoder.h)
// Copyright (c) 2020 Lester Lo 
// MIT License


#ifndef NSENCODER_H_
#define NSENCODER_H_

#define BTN_DEBOUNCE_MSTIME     30
#define BTN_PRESS_MSTIME        300
#define BTN_HOLD_MSTIME         600

#include <Arduino.h>
#include <Encoder.h>

class NSEncoder: public Encoder
{
public:
    //Constructor
    //Rotary encoder only
    NSEncoder
    (           
        uint8_t pinA, 
        uint8_t pinB, 
        uint8_t stepsPerNotch = 1
    );
    //Destructor
    ~NSEncoder();
    bool get_Position(int16_t &);
    int16_t get_diffPosition();
    void reset();
private:
    // Pin Define 
    const uint8_t _stepsPerNotch;

    int16_t enc_old_pulse = -999; //Store the vectorize pulse number
    int16_t enc_old_step = 0; //Store the vectorize pulse number
};

class NSEncoder_btn: public NSEncoder
{
public:
    //Type
    typedef enum {
        OPEN,
        PRESSED,
        DOUBLE_PRESSED,
        HELD,
        RELEASED
    } BTN_STATE;
    //Rotary encoder with button
    NSEncoder_btn
    (  
        uint8_t pinA, 
        uint8_t pinB, 
        uint8_t pin_BTN,
        uint8_t stepsPerNotch = 1, 
        bool enable_doublePress = false,
        bool pin_activeType = LOW
    );
    ~NSEncoder_btn();
    uint16_t getBtnDebounceInterval();
    void setBtnDebounceInterval(uint16_t);
    void setBtnPressInterval(uint16_t);
    void setBtnHoldInterval(uint16_t);

    void btn_task();
    BTN_STATE get_Button();

private:
    //Variable
    const uint8_t _pin_BTN;
    const bool _pin_activeType;
    const bool _enable_doublePress;

    uint16_t _btn_debounce_interval;
    uint16_t _btn_press_interval;
    uint16_t _btn_hold_interval;

    uint16_t _btn_press_tick;
    uint16_t _btn_hold_tick;

    //Normal Button check variable
    uint16_t keyPressed_tick;

    //Double Press handling Variable
    uint8_t key_isPressed;
    bool doublePress_startCalTick_flag;
    uint16_t doublePress_tick_counter;

    //Output status variable
    volatile BTN_STATE btn_status; //Share with isr call and normal call

    //function
    void reset_doublePress_counter();
};

#endif
