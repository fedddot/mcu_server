FROM gcc:11.4.0

RUN apt-get update
RUN apt-get install -y cmake
RUN apt-get install -y gdb
RUN apt-get install -y git
RUN apt-get install -y wget


WORKDIR /usr/app/external

RUN git clone --depth 1 --branch openssl-3.4.0 https://github.com/openssl/openssl.git openssl
RUN cd openssl && ./Configure --release && make -j4 && make install

RUN git clone --depth 1 --branch v1.15.2 https://github.com/google/googletest.git googletest
RUN mkdir -p googletest/build && \
cd googletest/build && \
    cmake .. && make install
    
RUN git clone --depth 1 --branch 1.9.6 https://github.com/open-source-parsers/jsoncpp.git jsoncpp
RUN mkdir -p jsoncpp/build && \
    cd jsoncpp/build && \
    cmake .. && make install

RUN apt-get install -y libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev
RUN git clone --depth 1 --branch v2.10.19 https://github.com/microsoft/cpprestsdk.git cpprestsdk
RUN cd cpprestsdk && git submodule update --init
RUN mkdir -p cpprestsdk/build && \
    cd cpprestsdk/build && \
    cmake .. && make install

ENV LD_LIBRARY_PATH=/usr/local/lib

# Sources root dir should be boundled to this location when running the container
WORKDIR /usr/app/src