#!/bin/bash

Xephyr -keybd ephyr,,,xkbmodel=evdev -br -reset -screen 1280x720x24 -dpi 96 :2 &
sleep 1

export DISPLAY=:2.0
gdb -ex run $(dirname $0)/twindy-dbg
killall Xephyr
