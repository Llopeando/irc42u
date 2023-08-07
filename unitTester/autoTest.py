import subprocess
from subprocess import Popen
import time
import random
from typing import List, Any
from dataclasses import dataclass

def IDgen():
	ID = 0
	while True:
		yield ID
		ID += 1

@dataclass
class Client:
	fd : Popen
	ID : int

def spawn_nc_client(server_address, port):
	nc_cmd = ['nc', server_address, str(port)]
	return  subprocess.Popen(nc_cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)


commandPool = [
	"QUIT",
	#"JOIN #a",
	#"PART #a",
	#"PRIVMSG #a",
	#"PRIVMSG ullorent hahhahaaha",
	#"TOPIC #a topicazoooo",
 	#"TOPIC #a",
	#"NOTICE #a aaaaaaa",
	#"MOTD",
	#"HELP",
]

def send(client, message):
	try:
		client.stdin.write(message + "\n")
		client.stdin.flush()
	except BrokenPipeError:
		print(f"ERROR broken pipe: message: {message}")


def randomCmd():
	return commandPool[random.randint(0,len(commandPool) - 1)]

def cmdAll(clients : [], cmd: str, fFilter= None):
	for index, client in enumerate(clients):
		if fFilter is not None:
			if fFilter(index):
				send(client, cmd)
		else:
			send(client, cmd)

# Example usage
server_address = "localhost"
port = 12345
password = "PASS hola"

nick_messages = "NICK client"
user_messages = "USER client"

numOfClients = 50

clients = []

for index in range(0, numOfClients):
	clients.append(spawn_nc_client(server_address, port))
	send(clients[index], password)
	send(clients[index], nick_messages + str(index))
	send(clients[index], user_messages + str(index) + " 0 * : a")



even = lambda index: index % 2 == 0
odd = lambda index: index % 2 != 0

cmdAll(clients, "JOIN #b")

while True:
	index_to_quit = []
	for index,client in enumerate(clients):
		command = randomCmd()
		print(f"[{index}]Command: {command}")
		send(client, command)
		if command == "QUIT":
			index_to_quit.append(index)
	time.sleep(2)
	i = 0
	for index in index_to_quit:
		clients[index - i].terminate()
		clients.pop(index - i)
		clients.append(spawn_nc_client(server_address, port))
		send(clients[len(clients) - 1 - i], password)
		send(clients[len(clients) - 1 - i], nick_messages + str(index - 1))
		send(clients[len(clients) - 1 - i], user_messages + str(index - 1) + " 0 * : a")
		i += 1
#def minishell()
#
#minishell()

while True:
	pass


