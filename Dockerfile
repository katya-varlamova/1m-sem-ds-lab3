FROM ubuntu:20.04
ENV TZ=Europe/Moscow \
    DEBIAN_FRONTEND=noninteractive

RUN apt update -y && apt install -y gcc g++ gcc-10 g++-10 pip clang-format postgresql postgresql-contrib \
    cmake libpqxx-dev libspdlog-dev libboost1.71-all-dev libssl-dev libjsoncpp-dev git && \
    git clone https://github.com/arun11299/cpp-jwt.git && mkdir cpp-jwt/build && cd cpp-jwt/build && \
    cmake -DCPP_JWT_BUILD_TESTS=False ../ && make && make install && cd / && \
    git clone https://github.com/jbeder/yaml-cpp.git && mkdir yaml-cpp/build && cd yaml-cpp/build && \
    cmake ../ && make && make install && cd / 

RUN apt install -y wget
RUN wget https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-linux-x86_64.tar.gz && \
    tar -xzvf cmake-3.21.3-linux-x86_64.tar.gz && mv cmake-3.21.3-linux-x86_64 /opt/cmake-3.21.3 && \
    ln -s /opt/cmake-3.21.3/bin/cmake /usr/local/bin/cmake && \
    git clone https://github.com/oatpp/oatpp.git && mkdir oatpp/build && cd oatpp/build && \
    cmake ../ && make && make install && cd / 
RUN python3 -m pip install requests

COPY src/ThirdParty/boostDi/di.hpp /usr/include/boost/
RUN  git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git && cd AMQP-CPP && mkdir build && cd build && cmake .. && cmake --build . --target install