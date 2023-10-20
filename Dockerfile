FROM gcc AS base

RUN \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive \
  apt-get install -y \
  fuse \
  libfuse-dev \
  libtorrent-rasterbar-dev \
  libcurl4-openssl-dev \
  && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/*

COPY . /src/btfs/
WORKDIR /src/btfs/

RUN \
  autoreconf -fvi && \
  ./configure

RUN make

ENTRYPOINT [ "btfs", "-f" ]
FROM base AS dev
RUN \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive \
  apt-get install -y \
  gdb locales cmake \
  && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/* && \
  localedef -i C -c -f UTF-8 -A /usr/share/locale/locale.alias C.UTF-8
ENV LC_TYPE=C.UTF-8 LC_ALL=C.UTF-8
FROM base
RUN make install