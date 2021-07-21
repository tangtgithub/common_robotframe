#!/bin/bash
#参数1：编译目录，目前只支持当前目录，默认build
#参数2：机器人类型,值为数字,默认0,即手动选择机器人类型.若值为99表示使用cmakelist内的机器人类型

ROBOT_TYPE_NUMBER=0
ROBOT_TYPE=""
BUILD_TYPE=""
CurrentScriptDir=$PWD
robotself=$CurrentScriptDir
cpu_num=7

if [ $# -eq 2 ];then
    BUILD_DIR=$1
    ROBOT_TYPE_NUMBER=$2
elif [ $# -eq 1 ];then
    BUILD_DIR=$1
else
    BUILD_DIR=$robotself/build
fi

switchRobotType(){
    if [ $ROBOT_TYPE_NUMBER -eq 1 ]; then ROBOT_TYPE="ROBOT_TEST"
    elif [ $ROBOT_TYPE_NUMBER -eq 2 ]; then ROBOT_TYPE="ROBOT_OTHER"
    fi
}

selectRobotType(){
    echo "请选择机器人类型:"
    echo "1、测试机器人"
    echo "2、其他机器人"
    echo "回车、使用cmakelist里面的机器人类型"
    echo "请输入1-2中的任意整数或者直接回车:"
    while true
    do
        read ROBOT_TYPE_NUMBER
        if [ "$ROBOT_TYPE_NUMBER" == "" ];then
            ROBOT_TYPE_NUMBER=99
            break
        elif (( $ROBOT_TYPE_NUMBER > 0 && $ROBOT_TYPE_NUMBER < 14 ));then
            break
        else
            echo "请输入1-13中的任意整数或者直接回车:"
        fi
    done
}

selectBuildType(){
    echo "请选择编译类型类型:"
    echo "1、Debug"
    echo "2、Release"
    while true
    do
        read BUILD_TYPE_TEMP
        if [ $BUILD_TYPE_TEMP -eq 1 ];then
            BUILD_TYPE="Debug"
            break
        elif [ $BUILD_TYPE_TEMP -eq 2 ];then
            BUILD_TYPE="Release"
            break
        else
            echo "请输入1或者2"
        fi
    done
}

touchBuildDir(){
    if [ ! -d ${BUILD_DIR} ];then
        mkdir -p ${BUILD_DIR}
    fi
}

getCpuNum(){
    cpu_num=`cat /proc/cpuinfo | grep processor | wc -l`
    if [ $cpu_num -eq 0 ];then
        cpu_num=7
    else
        cpu_num=`expr $cpu_num \* 8 / 10`
    fi
}

main(){
    if [ $ROBOT_TYPE_NUMBER -eq 0 ];then
        selectRobotType
    fi
	selectBuildType
    switchRobotType
    touchBuildDir
    cd ${BUILD_DIR}
    if [ $ROBOT_TYPE_NUMBER -ne 99 ];then
        cmake -D ROBOT_TYPE=${ROBOT_TYPE} -D CMAKE_BUILD_TYPE=${BUILD_TYPE} ..
    else
	    cmake ..
    fi
    getCpuNum
    make -j$cpu_num
    exit 0
}

main
