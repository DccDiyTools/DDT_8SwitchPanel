#!/bin/sh

if [ ! -d .kibot ]; then
  git clone https://github.com/DccDiyTools/kibot.git .kibot
else
  git -C .kibot pull
fi

docker run -it --rm \
  -v $(pwd):/work \
  -v $(pwd)/.kibot:/opt/kibot \
  -w /work \
  -e KIBOT_ROOT_SCRIPTS=/opt/kibot/kibot \
  ghcr.io/inti-cmnb/kicad8_auto_full:latest 