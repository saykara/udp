# UDP Task
## How to run

### 1- Docker
```sh
docker compose build
docker compose up
```

(Frequency and buffer size can be change by environmental variables inside docker-compose.yml)

### 2- Locally
#### 2.0 Environmental variables sets
```sh
export SERVER_IP = "172.18.0.3"
export PORT = 8080
export BUFFER = 1024
export FREQUENCY = 10
```

#### 2.1 Local build
```sh
cd udp
mkdir build && cd build
cmake ..
make
node app
```

#### 2.2 Local Run

**Terminal 1 - Run server - (From root of th repo):**
```sh
./build/server/server 
```

**Terminal 2 - Run client - (From root of th repo):**
```sh
./build/client/client 
```

