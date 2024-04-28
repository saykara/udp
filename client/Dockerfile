FROM ubuntu:latest

WORKDIR /app

# COPY client/src /app/src
# COPY client/CMakeLists.txt /app/CMakeLists.txt
COPY . .

RUN apt-get update && apt-get install -y cmake \
   g++

RUN cd /app && cmake . && make

EXPOSE 3000

CMD ["./client"]