FROM debian:buster

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y make gcc g++ gdb
RUN apt-get install -y git
RUN apt-get install -y wget
RUN apt-get install -y libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev
RUN apt-get install -y libwebsocketpp-dev
RUN apt-get install -y python3
RUN apt-get install -y libcpprest-dev
RUN apt-get install -y protobuf-compiler
RUN apt-get install -y lsb-release gnupg software-properties-common
RUN apt-get install -y locales

RUN rm -rf /var/lib/apt/lists/* && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV LANG=en_US.utf8

WORKDIR /usr/app/external/cmake
ARG CMAKE_VERSION=3.31.4
ARG CMAKE_PLATFORM=linux-x86_64
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-${CMAKE_PLATFORM}.sh
RUN chmod +x cmake-${CMAKE_VERSION}-${CMAKE_PLATFORM}.sh
RUN echo "y" | sh cmake-${CMAKE_VERSION}-${CMAKE_PLATFORM}.sh --prefix=/usr
ENV PATH=${PATH}:/usr/cmake-${CMAKE_VERSION}-${CMAKE_PLATFORM}/bin


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

# Sources root dir should be mounted to this location when running the container
WORKDIR /usr/app/src

CMD ["/bin/bash", "-c"]