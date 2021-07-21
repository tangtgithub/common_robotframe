#!/bin/bash

if [ $# == 0 ];then
    echo "arg num valid"
    exit -1
fi


BcsDeployDir=$1

cd $BcsDeployDir/lib
if [ -f "libhiredis.so.0.13" ];then
    rm -f libhiredis.so
    ln -s libhiredis.so.0.13 libhiredis.so
fi

if [ -f "libprotobuf.so.24.0.0" ];then
    rm -f libprotobuf.so.24
    rm -f libprotobuf.so
    ln -s libprotobuf.so.24.0.0 libprotobuf.so.24
    ln -s libprotobuf.so.24 libprotobuf.so
fi

if [ -f "libvdpau.so.1.0.0" ];then
    rm -f libvdpau.so.1
    rm -f libvdpau.so
    ln -s libvdpau.so.1.0.0 libvdpau.so.1
    ln -s libvdpau.so.1 libvdpau.so
fi

if [ -f "libva-x11.so.1.3900.0" ];then
    rm -f libva-x11.so.1
    rm -f libva-x11.so
    ln -s libva-x11.so.1.3900.0 libva-x11.so.1
    ln -s libva-x11.so.1 libva-x11.so
fi

if [ -f "libva-drm.so.1.3900.0" ];then
    rm -f libva-drm.so.1
    rm -f libva-drm.so
    ln -s libva-drm.so.1.3900.0 libva-drm.so.1
    ln -s libva-drm.so.1 libva-drm.so
fi

