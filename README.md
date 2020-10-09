# tjfoc-sprout

机密交易sprout方案



项目在linux环境下运行，利用cmake构建的项目。

编译说明

| 基本环境 | 版本                                |
| -------- | ----------------------------------- |
| g++      | (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0 |
| gcc      | (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0 |
| cmake    | 3.10.2                              |

使用vcpkg管理部分公共第三方依赖包

安装vcpkg

```bash
sudo apt-get install build-essential tar curl zip unzip
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
sudo ln -s /home/chenxu/vcpkg/vcpkg /usr/bin      #建立软链接，注意路径修改， /home/chenxu/vcpkg/vcpkg 修改为vcpkg执行文件的绝对路径
```

使用vcpkg安装依赖包

```bash
vcpkg install openssl

vcpkg install secp256k1

vcpkg install libsodium

vcpkg install gtest         

vcpkg install boost
```

安装其他zcash相关依赖

```bash
git clone https://github.com/zcash/zcash.git
cd zcash
sudo bash zcutil/fetch-params.sh    #会在${HOME}下生成.zcash-params目录，包含700MB左右的三个配置文件sapling-output.params  sapling-spend.params  sprout-groth16.params
```



tjfoc-sprout源代码编译安装

修改vcpkg引用地址

```bash
cd tjfoc-sprout
vim CMakeList.txt
第二行VCPKG_ROOT变量中路径修改为vcpkg实际目录后保存
#set(VCPKG_ROOT "/home/chenxu/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
```



编译运行

```bash
#cd tjfoc-sprout  在源代码目录下执行
mkdir build
cd build
cmake ..  
make
cd test
./gtest_simple_test
```

