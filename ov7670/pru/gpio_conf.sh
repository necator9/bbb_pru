#!/bin/bash
# To configure all the pins correctly, the emmc and audio uboot overlays must be disabled
# Without an emmc overlay only booting from SD card is possible.
# Pin configuration can be checked via https://github.com/mvduin/bbb-pin-utils

export TARGET=gpio_in.pru1
echo TARGET=$TARGET

pins_in="P8_41 P8_45 P8_42 P8_39 P8_40 P8_27 P8_29 P8_28 P8_30 P8_21" 
pins_out="P8_20 P8_46 P8_43 P8_44"
pins_i2c="P9_19 P9_20"
pins_pwm="P8_34"


function config_pin {
    pins=$1
    mode=$2
    for pin in $pins
    do
        echo $pin $mode
        config-pin $pin $mode
    done
} 

config_pin "$pins_in" pruin
config_pin "$pins_out" pruout
config_pin "$pins_i2c" i2c
config_pin "$pins_pwm" pwm
