#!/bin/sh
cd "$(dirname "$0")"
cclean
make
if [ $? -ne 0 ]; then
	echo "Error, failed to make unitTester"
	exit 1
fi
./unitTester
make fclean

cd ..
make fclean
make MINISHELL=1
./ircserv 12345 hola &
SERVER_PID=$!
if [ SERVER_PID -ne 0 ]; then
	echo "Error, failed to make irc!"
	exit 1
fi

cd unitTester
python3 autoTest.py &
PYTHON_PID=$!
if [ PYTHON_PID -ne 0 ]; then
	kill $SERVER_PID 
	echo "Error, failed to execute autoTest.py!"
	exit 1
fi

wait

kill $PYTHON_PID
kill $SERVER_PID

cd ..
make fclean

exit 0
