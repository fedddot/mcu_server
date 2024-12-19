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

WORKDIR /usr/app/external

RUN git clone --depth 1 --branch v1.15.2 https://github.com/google/googletest.git googletest
ENV GOOGLE_TEST_PATH=/usr/app/external/googletest

RUN git clone --depth 1 --branch 1.9.6 https://github.com/open-source-parsers/jsoncpp.git jsoncpp
ENV JSONCPP_PATH=/usr/app/external/googletest

# ENV LD_LIBRARY_PATH=/usr/local/lib

# Sources root dir should be boundled to this location when running the container
WORKDIR /usr/app/src

CMD ["/bin/bash", "-c"]