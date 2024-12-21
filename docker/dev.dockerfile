FROM gcc:11.4.0

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y cmake
RUN apt-get install -y gdb
RUN apt-get install -y git
RUN apt-get install -y wget
RUN apt-get install -y libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev
RUN apt-get install -y libwebsocketpp-dev
RUN apt-get install -y python3
RUN apt-get install -y lsb-release software-properties-common gnupg

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

WORKDIR /usr/app/external/cpprestsdk
RUN wget https://github.com/microsoft/cpprestsdk/archive/refs/tags/v2.10.15.tar.gz
ENV CPPREST_ARCHIVE_PATH=/usr/app/external/cpprestsdk/v2.10.15.tar.gz

# Sources root dir should be mounted to this location when running the container
WORKDIR /usr/app/src

CMD ["/bin/bash", "-c"]