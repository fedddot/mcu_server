FROM cmake:bookworm

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y make gcc g++ gdb
RUN apt-get install -y git
RUN apt-get install -y wget
RUN apt-get install -y libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev
RUN apt-get install -y libwebsocketpp-dev
RUN apt-get install -y python3 python3-pip
RUN apt-get install -y libcpprest-dev

RUN apt-get install -y lsb-release gnupg software-properties-common
RUN apt-get install -y locales

RUN rm -rf /var/lib/apt/lists/* && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV LANG=en_US.utf8
ENV LC_ALL=en_US.utf8

ARG TARGETPLATFORM=x86_64

WORKDIR /usr/app/external/llvm
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 18
ENV PATH=${PATH}:/usr/lib/llvm-18/bin

WORKDIR /usr/app/external/googletest
RUN wget https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
ENV GOOGLE_TEST_ARCHIVE_PATH=/usr/app/external/googletest/v1.15.2.tar.gz

WORKDIR /usr/app/external/jsoncpp
RUN wget https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.6.tar.gz
ENV JSONCPP_ARCHIVE_PATH=/usr/app/external/jsoncpp/1.9.6.tar.gz

WORKDIR /usr/app/external
RUN git clone --branch=0.4.9 https://github.com/nanopb/nanopb.git nanopb
ENV NANOPB_SRC_PATH=/usr/app/external/nanopb

# Sources root dir should be mounted to this location when running the container
WORKDIR /usr/app/src

CMD ["/bin/bash", "-c"]