#!/usr/bin/env bash

docker run --gpus=all -eDISPLAY=$DISPLAY -v/tmp/.X11-unix:/tmp/.X11-unix:rw \
  --name dustlab -p 7775:22 -p 7777:7777 -p 8888:80 --cap-add sys_ptrace --privileged --rm \
  -d dustimg
