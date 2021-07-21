#!/bin/bash
str="SLRobot"

sleep 15

while true
do
    result=$(ps aux | grep "SLRobot" | grep -v grep)
	if [[ "$result" != "" ]]
	then
            sleep 1
	else
         cd $proPath
	    ./start.sh &
	    sleep 1
	fi
done
