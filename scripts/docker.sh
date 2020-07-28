#!/bin/sh

BRD_CONTAINER=${BRD_CONTAINER:-brd-alpine:3}
BRD_DOCKERFILE=Dockerfile.$(echo $BRD_CONTAINER | sed -e 's/:/-/' -e 's/^brd-//'); \
    [ $? -eq 0 ] || exit 1
BRD_CONATAINER_HASH=$(docker images -q $BRD_CONTAINER); [ $? -eq 0 ] || exit 1

if [ "$BRD_FORCE_DOCKER_BUILD" == "1" ] || [ -z ${BRD_CONATAINER_HASH:+x} ]; then
    docker build -f $BRD_DOCKERFILE -t $BRD_CONTAINER . || exit 1
fi

docker run -it \
    -e "DISPLAY=unix$DISPLAY" \
    -v/tmp/.X11-unix:/tmp/.X11-unix \
    --device /dev/dri:/dev/dri \
    $BRD_CONTAINER || exit 1
