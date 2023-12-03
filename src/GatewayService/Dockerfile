FROM katyavarlamova/ds-project as builder

COPY ./GatewayService /src
WORKDIR /src
RUN mkdir build && cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-10 -S./ -B./build && \
    cd build && make

FROM katyavarlamova/ds-project

COPY --from=builder /src/ /app
RUN mv /app/publish.py /app/build/main/publish.py
ENTRYPOINT ["/app/build/main/main"]
