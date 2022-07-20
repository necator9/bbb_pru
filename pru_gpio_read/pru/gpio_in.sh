#!/bin/bash
#
export TARGET=gpio_in.pru0
echo TARGET=$TARGET

# Configure the PRU pins based on which Beagle is running
machine=$(awk '{print $NF}' /proc/device-tree/model)
echo -n $machine
if [ $machine = "Black" ]; then
    echo " Found"
    pins_in="P9_31 P9_30 P9_29"
    pins_out=""
else
    echo " Not Found"
    pins_in=""
    pins_out=""
fi

for pin in $pins_in
do
    echo $pin
    config-pin $pin pruin
done

for pin in $pins_out
do
    echo $pin
    config-pin $pin pruout
done


