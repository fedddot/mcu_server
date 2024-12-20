FROM gcc:11.4.0

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y cmake
RUN apt-get install -y gdb
RUN apt-get install -y git
RUN apt-get install -y wget
RUN apt-get install -y openssl
RUN apt-get install -y libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev
RUN apt-get install -y libcpprest-dev
RUN apt-get install -y python3
RUN apt-get install -y lsb-release wget software-properties-common gnupg

WORKDIR /usr/app/external

RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 18
ENV PATH=${PATH}:/usr/lib/llvm-18/bin

RUN wget https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
ENV GOOGLE_TEST_ARCHIVE_PATH=/usr/app/external/v1.15.2.tar.gz

RUN wget https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.6.tar.gz
ENV JSONCPP_ARCHIVE_PATH=/usr/app/external/1.9.6.tar.gz

# ENV LD_LIBRARY_PATH=/usr/local/lib

# Sources root dir should be boundled to this location when running the container
WORKDIR /usr/app/src

CMD ["/bin/bash", "-c"]