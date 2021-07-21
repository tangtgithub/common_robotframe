export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib/
pkill -15 SLRobot
nohup ./SLRobot > /dev/null 2>&1 &
