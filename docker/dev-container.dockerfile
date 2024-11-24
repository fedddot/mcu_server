FROM gcc:latest

RUN apt-get update
RUN apt-get install -y cmake
RUN apt-get install -y gdb
RUN apt-get install -y libcpprest-dev
RUN apt-get install -y git
RUN apt-get install -y libssl-dev

WORKDIR /usr/app/external
RUN git clone --depth 1 --branch v1.15.2 https://github.com/google/googletest.git googletest
RUN mkdir -p googletest/build && \
    cd googletest/build && \
    cmake .. && make install

RUN git clone --depth 1 --branch 1.9.6 https://github.com/open-source-parsers/jsoncpp.git jsoncpp
RUN mkdir -p jsoncpp/build && \
    cd jsoncpp/build && \
    cmake .. && make install

# Sources root dir should be boundled to this location when running the container
WORKDIR /usr/app/src