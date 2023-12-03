FROM ubuntu:20.04
ENV TZ=Europe/Moscow     DEBIAN_FRONTEND=noninteractive

RUN apt update -y && apt install -y gcc g++ gcc-10 g++-10 git clang-format  cmake libpqxx-dev libspdlog-dev libboost-all-dev libjsoncpp-dev libssl-dev  &&  git clone https://github.com/jbeder/yaml-cpp.git   &&      mkdir yaml-cpp/build     &&    cd yaml-cpp/build     &&        cmake ../    &&     make    &&     make install  &&       cd / 

RUN git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git && cd AMQP-CPP  && cmake -DAMQP-CPP_LINUX_TCP=ON . && make && make install && cd /
    

# Install wget and download a specified version of cmake
RUN apt-get install -y wget     &&        wget https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-linux-x86_64.tar.gz      &&       tar -xzvf cmake-3.21.3-linux-x86_64.tar.gz   &&      mv cmake-3.21.3-linux-x86_64 /opt/cmake-3.21.3      &&       ln -s /opt/cmake-3.21.3/bin/cmake /usr/local/bin/cmake

RUN apt-get install -y build-essential

# Clone and build oatpp
RUN git clone https://github.com/oatpp/oatpp.git    &&     mkdir oatpp/build     &&    cd oatpp/build &&            cmake ../    &&     make    &&     make install     &&    cd /

RUN apt-get -y install python3-pip

COPY src/ThirdParty/boostDi/di.hpp /usr/include/boost/.


RUN apt-get install -y libpoco-dev

RUN apt-get install -y python3

RUN pip3 install requests
