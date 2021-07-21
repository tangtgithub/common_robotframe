# common_robotframe


# 代码编译

## 1、编译

编译步骤：执行compile.sh脚本，根据脚本提示进行编译。


# 编译环境配置

## *1、Ubuntu 18.04 环境配置*

### ffmpeg v3.4.8

```
sudo apt install ffmpeg
```

### mosquitto v1.4.15

```
sudo apt install libmosquitto-dev -y
```

### jsoncpp v1.7.4

```
sudo apt install libjsoncpp-dev -y
```

### crypto++ v5.6.4

```
sudo apt install libcrypto++6 libcrypto++-dev -y
```

### curl v7.58.0

```
sudo apt install libcurl4-openssl-dev -y
```

### ssl v1.1.1

```
sudo apt install libssl-dev -y
```

### redis v4.0.9

```
sudo apt install redis-server  libhiredis-dev
```

### sqlite3 v3.22

```
sudo apt install sqlite3 libsqlite3-dev
```

### boost库 v1.65.1

```
sudo apt install libboost-all-dev
```

### cmake v3.10.2

```
sudo apt install cmake build-essential
```

### git

```
sudo apt install git
```

### ccache

```
sudo apt install ccache
```



## *2、Ubuntu 16.04 环境配置*

### ffmpeg v4.1

```
通过源码手动安装
依赖：sudo apt install yasm libsdl2-dev libx264-dev libx265-dev
1. cd depend_lib_src/protobuf-3.13.0
2. ./configure   --enable-shared  --prefix=/usr/local/ffmpeg  --enable-gpl --enable-libx264  --enable-libx265   
3. make
4. make install
5. export LD_LIBRARY_PATH=/usr/local/ffmpeg/lib:$LD_LIBRARY_PATH
6. ldd ffmpeg 
7. ffmpeg --version
```

### mosquitto v1.4.8

```
sudo apt install  libmosquitto-dev -y
```

### mosquittopp v1.4.8

```
sudo apt install  libmosquittopp-dev -y
```

### jsoncpp v1.7.2

```
sudo apt install  libjsoncpp-dev -y
```

### crypto++ v5.6.1

```
sudo apt install libcrypto++-dev -y
```

### curl v7.47.0

```
sudo apt install libcurl4-openssl-dev -y
```

### ssl v1.0.2g

```
sudo apt install libssl-dev -y
```

### redis v3.0.6

```
sudo apt install redis-server  libhiredis-dev
```

### sqlite3 v3.11

```
apt install sqlite3 libsqlite3-dev
```

### boost库 v1.58.0

```
apt install libboost-all-dev
```

### cmake v3.5.1

```
apt install cmake build-essential
```
### git

```
sudo apt install git
```

### ccache

```
sudo apt install ccache
```

### protobuf

```
cd depend_lib_src/protobuf-3.13.0
./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig # refresh shared library cache.
```

### gcc7.4

```
wget http://ftp.gnu.org/gnu/gcc/gcc-7.4.0/gcc-7.4.0.tar.gz
tar -zxvf gcc-7.4.0.tar.gz
cd gcc-7.4.0
## 如果需要更换下载源，请修改文件中的base_url的值
#vim ./contrib/download_prerequisites
## 下载依赖包
./contrib/download_prerequisites
mkdir build
cd build
../configure -enable-checking=release -enable-languages=c,c++ -disable-multilib
make
make install
```



## 3、第三方SDK

### alibabacloud-oss-cpp-sdk 1.9.0
`github`：

```
https://github.com/aliyun/aliyun-oss-cpp-sdk/releases
```
`下载地址`：

```
wget https://github.com/aliyun/aliyun-oss-cpp-sdk/archive/1.9.0.zip -O aliyun-oss-cpp-sdk-1.9.0.zip
```



# *markdown编辑器 typora*

windows下载地址：https://typora.io/#windows
Linux下载地址：https://typora.io/#linux



# *Log输出到屏幕*

```
cmake -DLOG="STDLOG" ..
```
