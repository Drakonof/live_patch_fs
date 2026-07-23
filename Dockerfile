FROM ubuntu:22.04

LABEL maintainer="a.shimko.dev@gmail.com"
LABEL description="LivePatchFs build environment"
LABEL version="1.0"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update 

RUN apt-get install -y \
	locales cmake g++ libfuse3-dev libprotobuf-dev protobuf-compiler \
	libgrpc++-dev grpc++ pkg-config ninja-build vim build-essential \
	fuse3 \
	&& rm -rf /var/lib/apt/lists/* 

RUN locale-gen en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8

ARG USER_NAME=live_patch_fs
ARG USER_UID=1000
ARG USER_GID=1000

RUN groupadd -g $USER_GID $USER_NAME && \
	useradd -m -u $USER_UID -g $USER_GID -s /bin/bash $USER_NAME && \
	echo "$USER_NAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

WORKDIR /home/$USER_NAME/
RUN chown -R $USER_NAME:$USER_NAME /home/$USER_NAME
USER $USER_NAME

CMD ["/bin/bash"]
