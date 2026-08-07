#!/bin/bash

docker_build () {
	docker build -t live_patch_fs_docker .	
}

build () {
	docker run --rm -it -v \
	$(pwd):/home/live_patch_fs live_patch_fs_docker bash -c \
	"mkdir -p build && cd build && cmake .. && make"
}

install () {
	mkdir -p /tmp/livepatch && \
	./build/fuse_driver/livepatch_fuse /tmp/livepatch
}

remove () {
	fusermount3 -u /tmp/livepatch
}

reinstall () {
	build
	remove
	install
}

help () {
	echo "TODO: help"
}

case "${1:-h}" in
	d) docker_build ;;
	b) build ;;
	i) install ;;
	r) remove ;;
	ri) reinstall ;;
	*) help ;;
esac
