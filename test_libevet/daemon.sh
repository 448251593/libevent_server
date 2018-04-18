#!/bin/bash
while true
do
netstat -ntpl|grep "7000"|grep -v "grep"
if [ $? -eq  0 ]; then 
	echo 	'runing'
else
	echo 'no running'
	killall exelibevent
	cd /opt/tcpserver/test_libevet/
	./exelibevent &
	cd -
fi

netstat -ntpl|grep "80"|grep -v "grep"
if [ $? -eq  0 ]; then 
	echo 	'runing'
else
	echo 'no running'
	killall exelibevent
	cd /opt/lampp/
	./lampp restart &
	cd -
fi


sleep 15
done
