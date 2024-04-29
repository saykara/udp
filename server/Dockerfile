FROM ubuntu:latest

WORKDIR /app

# COPY server/src /app/src
# COPY server/CMakeLists.txt /app/CMakeLists.txt
COPY . .

RUN apt-get update && apt-get install -y cmake \
    g++

RUN cd /app && cmake . && make

CMD ["./server"]

