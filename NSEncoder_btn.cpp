// Notched Shaft Encoder with button support cpp file (NSEncoder_btn.cpp)
// Copyright (c) 2020 Lester Lo 
// MIT License

#include "NSEncoder.h"


NSEncoder_btn::NSEncoder_btn
                    (   
                        uint8_t pinA, 
                        uint8_t pinB, 
                        uint8_t pin_BTN,
                        uint8_t stepsPerNotch, 
                        bool enable_doublePress,
                        bool pin_activeType
                    )
        :   NSEncoder(pinA, pinB, stepsPerNotch),
            //Initialize Parameter Variable
            _pin_BTN(pin_BTN), 
            _enable_doublePress(enable_doublePress),
            _pin_activeType(pin_activeType),

            //Initialize Private variable
            //Normal Button check variable
            keyPressed_tick(0),
            //Double Press handling Variable
            key_isPressed(false),
            doublePress_startCalTick_flag(false),
            doublePress_tick_counter(0),
            //Output status variable
            btn_status(NSEncoder_btn::OPEN)


{
    //Set the Arduino pinMode to Input
    pinMode(_pin_BTN, ((pin_activeType == LOW )? INPUT : INPUT_PULLUP)); //Set the button whether active high / active low

    //Set the button timing variable
    setBtnPressInterval(BTN_PRESS_MSTIME);
    setBtnHoldInterval(BTN_HOLD_MSTIME);
    setBtnDebounceInterval(BTN_DEBOUNCE_MSTIME);
}

NSEncoder_btn::~NSEncoder_btn()
{

}

void NSEncoder_btn::btn_task()
{
    if(doublePress_startCalTick_flag)
        doublePress_tick_counter++; //Count the number of double press tick

    //Handle Button event 
    bool reading = digitalRead(_pin_BTN);
    if(reading == _pin_activeType) //Receive Pressed signal
    {
        keyPressed_tick++;  //Count the keyPress tick
        if(keyPressed_tick > _btn_hold_tick)
        {
            btn_status = HELD; //Set the statue to HELD mode
            reset_doublePress_counter(); //Since enter HELD mode, ignore the doublePress event
        }
    }
    else //Non-Pressed Signal
    {   
        //If the key is also Pressed on the last job call
        if(keyPressed_tick)//Debounce, button held aleast 1 tick
        {
            //If the btn is held state
            if(btn_status == HELD) {
                btn_status = RELEASED; // Set to RELEASED Mode since the button not pressed
            }
            else
            {
                //Single Press mode
                if(!_enable_doublePress)
                {
                    btn_status = PRESSED;//Directly fire a Pressed signal
                    keyPressed_tick = 0;//Reset keyPress Counter since it is handled
                }
                //Double Press Mode
                else
                {
                    //Hold the Pressed signal to fire
                    doublePress_startCalTick_flag = true; //Start the double press tick counter
                    key_isPressed++;//Count this press
                }
                   
            } 
        }
        keyPressed_tick = 0;//Reset keyPress Counter since it is handled
    }

    //Post Action of the timer task

    //Special handling double Press action
    if(_enable_doublePress)
    {
        if( doublePress_startCalTick_flag && 
            (doublePress_tick_counter >= _btn_press_tick))//times up
        {
            if(key_isPressed == 1) //key is Pressed 1 times before
                btn_status = PRESSED;
            else //if(key_isPressed == 2)
                btn_status = DOUBLE_PRESSED; 

            //Reset the double Press Monitoring variable
            reset_doublePress_counter();
        }
    }
}

NSEncoder_btn::BTN_STATE NSEncoder_btn::get_Button()
{
    //Ensure the user read the last before change back to button OPEN statue
    NSEncoder_btn::BTN_STATE temp_btn_status = btn_status ;//Save the last state;
    if(btn_status != HELD)
    {
        btn_status = OPEN;
    }
    return temp_btn_status;
}

//Button Interval get / set function
uint16_t NSEncoder_btn::getBtnDebounceInterval()
{
    return _btn_debounce_interval;
}
void NSEncoder_btn::setBtnDebounceInterval(uint16_t btn_debounce_interval)
{
    _btn_debounce_interval = btn_debounce_interval;
    setBtnPressInterval(_btn_press_interval);
    setBtnHoldInterval(_btn_hold_interval);
}
void NSEncoder_btn::setBtnPressInterval(uint16_t btn_press_interval)
{
    _btn_press_interval = btn_press_interval;
    _btn_press_tick = btn_press_interval / _btn_debounce_interval;
}
void NSEncoder_btn::setBtnHoldInterval(uint16_t btn_hold_interval)
{
    _btn_hold_interval = btn_hold_interval;
    _btn_hold_tick = _btn_hold_interval / _btn_debounce_interval;
}

//Private Function
void NSEncoder_btn::reset_doublePress_counter()
{
    //Reset All double Press Monitoring variable
    key_isPressed = 0;
    doublePress_tick_counter = 0;
    doublePress_startCalTick_flag = false;    
}