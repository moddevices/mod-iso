#!/bin/bash

if [ "$TWINDY_SKIP_LOCAL_TEST"x != "1"x ]; then
  export TWINDY_SKIP_JACKD_START=1
  export TWINDY_LOCAL_TEST=1
fi

export PATH=$(pwd):$PATH

Xephyr -keybd ephyr,,,xkbmodel=evdev -br -reset -screen 1920x979x24 -resizeable -dpi 96 :2 &
sleep 1

export DISPLAY=:2.0
$(dirname $0)/twindy
killall mod-host Xephyr
