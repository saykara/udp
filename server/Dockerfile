FROM ubuntu:latest

WORKDIR /app

COPY . .

RUN apt-get update && apt-get install -y cmake \
    g++

RUN cd /app && mkdir -p build && cd build && cmake .. && make

CMD ["./build/server/server"]
