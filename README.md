# UDP Task
## How to run

### 1- Locally
#### 1.1 Local build
```sh
cd udp
mkdir build && cd build
cmake ..
make
node app
```

#### 1.2 Local Run

**Terminal 1 - Run server - (From root of th repo):**
```sh
./build/server/server 
```

**Terminal 2 - Run client - (From root of th repo):**
```sh
./build/client/client 
```

### 2- Docker
```sh
docker-compose build
docker-compose up
```