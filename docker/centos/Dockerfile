FROM centos:7.7.1908

ARG cmake_version=3.16.4
ARG boost_version=1.70.0
ARG rondb_version=21.04.7
ARG glibc_version=2.17
ARG arch=x86_64
ARG lib_ssl_version=1.1.1g
ARG userid=1000
ARG groupid=1000

RUN yum -y update; \
echo Installing GCC 8; \
yum -y install centos-release-scl; \
yum -y install devtoolset-8-gcc devtoolset-8-gcc-c++; \
yum -y install wget make git which perl-core libtemplate-perl zlib-devel

RUN cd; \
source scl_source enable devtoolset-8; \
wget --progress=bar:force https://www.openssl.org/source/openssl-${lib_ssl_version}.tar.gz; \
tar xf openssl-${lib_ssl_version}.tar.gz; \
cd openssl-${lib_ssl_version}; \
./config --prefix=/usr/local/ssl --openssldir=/usr/local/ssl shared zlib; \
make -j$(nproc); \
make install; \
echo "/usr/local/ssl/lib" >> /etc/ld.so.conf.d/openssl-${lib_ssl_version}.conf; \
ldconfig -v; \
cd ..; \
rm -rf openssl-${lib_ssl_version} 

RUN echo Installing cmake; \
source scl_source enable devtoolset-8; \
wget --progress=bar:force https://github.com/Kitware/CMake/releases/download/v${cmake_version}/cmake-${cmake_version}.tar.gz; \
tar xzf cmake-${cmake_version}.tar.gz; \
cd cmake-${cmake_version}; \
export OPENSSL_ROOT_DIR=/usr/local/ssl/; \
./bootstrap --prefix=/usr/local; \
make -j$(nproc); \
make install; \
cd ..; \ 
rm -r cmake-${cmake_version}*

RUN echo Installing Boost; \
BOOST_V=`echo ${boost_version} | sed s/'\.'/'_'/g`; \
source scl_source enable devtoolset-8; \
wget --progress=bar:force https://boostorg.jfrog.io/artifactory/main/release/${boost_version}/source/boost_${BOOST_V}.tar.gz; \
tar xzf boost_${BOOST_V}.tar.gz; \
cd boost_${BOOST_V}; \
./bootstrap.sh; \
./b2 link=static toolset=gcc cxxstd=14 install; \
cd ..; \
rm -r boost_${BOOST_V}*

# Use master to avoid compilation issues (https://github.com/Tencent/rapidjson/issues/1205)
# will update to new version when this is fixed to a version
RUN echo Installing RapidJson; \
source scl_source enable devtoolset-8; \
git clone https://github.com/Tencent/rapidjson.git; \
cd rapidjson; \
git checkout master; \
mkdir build; \
cd build; \
cmake ..; \
make install

RUN echo Installing MySQL Cluster; \
cd /usr/local; \
wget --progress=bar:force https://repo.hops.works/master/rondb-${rondb_version}-linux-glibc${glibc_version}-${arch}.tar.gz; \
tar xzf rondb-${rondb_version}-linux-glibc${glibc_version}-${arch}.tar.gz; \
mkdir -p mysql/lib; \
cp -r rondb-${rondb_version}-linux-glibc${glibc_version}-${arch}/include mysql/; \
cp -r rondb-${rondb_version}-linux-glibc${glibc_version}-${arch}/lib/libndbclient* mysql/lib/; \
rm -r rondb-${rondb_version}-linux-glibc${glibc_version}-${arch}*

RUN groupadd epipe --gid ${groupid}; \
useradd -ms /bin/bash epipe --uid ${userid} --gid ${groupid}; \
echo "source scl_source enable devtoolset-8" >> /home/epipe/.bashrc

USER epipe
CMD ["/usr/bin/scl", "enable", "devtoolset-8", "bash"]
