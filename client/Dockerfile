FROM ubuntu:latest

WORKDIR /app

# COPY client/src /app/src
# COPY client/CMakeLists.txt /app/CMakeLists.txt
COPY . .

RUN apt-get update && apt-get install -y cmake \
   g++

RUN cd /app && mkdir -p build && cd build && cmake .. && make

CMD ["./build/client"]