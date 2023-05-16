#include "../include/Server.hpp"
#include <chrono>
/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server(t_serverInput *serverInput):serverInfo(*serverInput)
{
	data =  UsersData(serverInfo);
	setCommands();
	channels.push_back(Channel("Lobby", data[(clientIt)0].getUsername() , &data));
	channels.push_back(Channel("Sala", data[(clientIt)0].getUsername() , &data));//temporal
	

}

Server::~Server()
{
	if ((status = close(data[(pollfdIt)0].fd) == SERVER_FAILURE))
	{
		perror("socket closing failed");
		exit(EXIT_FAILURE);
	}
}

/* ------------------------------------------------------------	*/
/*							MAIN								*/
/* ------------------------------------------------------------	*/


void	Server::run(){
	
	int events;
	
	listenConnection();
	while(1)
	{
		//printServerStatus();
		events = poll(data.getPollfdData(), static_cast<nfds_t>(data.size()), 200);
		if (events < 0)
		{
			perror("error-event detected");
			return;
		}
		if (events != 0)
			checkFds(events); 
	}
}



void	Server::checkFds(int events)
{
	pollfdIt i = 0;
	if (data[i].revents & POLLIN) //CONEXION REQUEST
	{
		printf("-------- INCOMING REQUEST RECEIVED --------\n");
		acceptConnection();
		events--;
	}
	for (i = 1; i < data.size() && events; i++) //INPUT REQUEST
	{
		handleEvents(i);
		events--;
	}
}


/* ------------------------------------------------------------ */
/*				LISTEN AND ACCEPT CONNECTION					*/
/* ------------------------------------------------------------ */


void	Server::listenConnection() {
	std::cout << "Server started, im listening" << std::endl;
	if (listen(data[(pollfdIt)0].fd, 3) == SERVER_FAILURE)
	{
		perror("listening process failure");
		exit(EXIT_FAILURE);
	}
}

void	Server::acceptConnection() {
	
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	struct pollfd new_client;

	std::cout << "The connection has been accepted, continuing" << std::endl;
	if ((new_client.fd = accept(data[(pollfdIt)0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
		return;
	}
	new_client.events = POLLOUT | POLLIN;
	data.addClient(new_client, Client());
	//sendMsgUser(data.size() - 1, "Welcome to - A O I R C - \n");
}

/* ------------------------------------------------------------ */
/*					    	NEW USER 							*/
/* ------------------------------------------------------------ */

void Server::handleNewUser(std::string &input, clientIt idx)
{
	//std::cout << "NEW USER\n";
	std::vector<std::string> lines = split(input, '\n');

	for (uint32_t i = 0; i < lines.size();i++)
	{
		std::vector<std::string> arguments = splitIrcPrameters(lines[i], ' ');
		if (i == 1) //NICK
			saveNick(arguments, data[idx]);
		else if (i == 2) //USER
			saveUser(arguments, data[idx]);
	}
	//data[idx].setState(CL_STATE_LS);
	//data.addClient(newUser);
}

void Server::saveNick(std::vector<std::string> &arguments, Client &client)
{
	client.setNickname(arguments[1]);
}

void Server::saveUser(std::vector<std::string> &arguments, Client &client)
{
	if (arguments.size() < 2)
	{
		std::cout << "failed to recieved input\n";
	}
	client.setUsername(arguments[1]);
	client.setRole(atoi(arguments[2].c_str()));
}



/* ---------------------------------------------------------------------------------------- */
/*						POLL() AND HANDLE EVENTS	 (incoming requests and inputs)	    //La nueva minishell		*/
/* ---------------------------------------------------------------------------------------- */


void Server::handleInput(clientIt index, std::string input) 
{
	std::vector<std::string>arguments = split(input, ' ');
	std::cout << "[" << arguments[0].c_str() << "]\n";
	std::cout << "[" << arguments[1].c_str() << "]\n";
	for (uint32_t i = 0; i < COMMANDS; i++)
	{
		if (commands.cmd[i] == arguments[0])
		{
			//std::cout << "command reached\n";
			(this->*(commands.func[i]))(index, arguments);
			return ;
		}
	}	
	//std::cout << "[Received message : " << input << "]\n";
	//AQUI TENEMOS QUE HACER BROADCAST
	
}

void Server::handleEvents(pollfdIt index)
{
	if (data[index].revents & POLLIN)
	{
		std::string input = readTCPInput(data[index].fd);
		std::cout << color::red << "\t\t[RECEIVED MESSAGE]\n\t\t" << color::boldwhite << input << color::reset;
		std::vector<std::string> lines = split(input, '\n');
		for (uint32_t i = 0; i < lines.size();i++)
		{
			handleInput((clientIt)index, lines[i]);
		}
	}
}


/* ---------------------------------------------------------------------------------------- */
/*						COMMAND FUNCTION													*/
/* ---------------------------------------------------------------------------------------- */


	//COMMAND FUNCTIONS
	
void	Server::nick(clientIt index, std::vector<std::string> &arguments)
{
	data[index].setNickname(arguments[1]);
	std::cout << "----------New nick changed: " << data[index].getNickname() << "]\n";

}

void	Server::user(clientIt index, std::vector<std::string> &arguments)
{
	//std::cout << "USER REACHED[" << arguments[1] << "]\n";
	data[index].setUsername(arguments[1]);
	//sendMsgUser(data[(pollfdIt)index].fd, message);
	std::string message = "001 " + data[index].getNickname() + " :Welcome to A O I R C\n" ;
	sendMsgUser(index, message);
	//std::string welcome = "001 " + data[index].getNickname() + " :Welcome to A O I R C server\n";
	//sendMsgUser(data[(pollfdIt)index].fd, message);
	std::cout << "----------New user changed: " << data[index].getUsername() << "]\n";

}


void	Server::privmsg(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	//aqui tiene que mirar argument[1] : #channel o username 

	std::string message = ":" + data[index].getNickname() +  " " +  arguments[0] + " " + data[index].getNickname() + " :" + joinStr(arguments, 2) + "\r\n";
 
	if (arguments[1][0] == '#') 	//to a CHANNEL 
	{
		uint32_t channel = findChannel(arguments[1].substr(1, arguments.size() - 1));
		if(!channel)
			return;
		channels[channel].broadcast(index, message);
	}
	else 	//to an USER 
	{
		clientIt user = data.findUsername(arguments[1]);
		if (!user)
			return;
	//	std::cout << "----------Mesage to : " << data[user].getNickname() << ": "<< message <<"]\n";
		sendMsgUser(user, message);
	}
}





///////////////////////////PROXIMAMENTE

void	Server::join(clientIt index, std::vector<std::string> &arguments)
{

	uint32_t channel = findChannel(arguments[1].substr(1, arguments[1].size() - 2));
	if(!channel)
			return;
	channels[channel].addClient(index);
	//////////ARREGLAR AQUIIIIIIII////////// TODO TIENE SALTO DE LINEA 
	std::string message =  data[index].getNickname() +  "has joined the channel\r\n";
	
	channels[channel].broadcast(0, message);
}


void	Server::part(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}

void	Server::notice(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}


void	Server::quit(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}


void	Server::topic(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}


void	Server::mode(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}


void	Server::names(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}


void	Server::whois(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}

void	Server::kick(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}

void	Server::away(clientIt index, std::vector<std::string> &arguments)
{
	(void)arguments;
	std::cout << "DENTRO DE AWAY" << std::endl;
	if (data[index].getAwayStatus() == true) {
		data[index].setAwayStatus(false);
		std::string message = ":10.13.8.1 PRIVMSG " + data[index].getNickname() + " :You are no longer away.\r\n";
		sendMsgUser(index, message);
	}
	data[index].setAwayStatus(true);
	std::string message = ":10.13.8.1 PRIVMSG " + data[index].getNickname() + " :You are now away.\r\n";
	sendMsgUser(index, message);
}
	
void	Server::invite(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}


void	Server::ping(clientIt index, std::vector<std::string> &arguments)
{
//	(void)index;
	(void)arguments;
	std::cout << "PING RECEIVED\n";
	std::string message = ":10.13.8.1 PRIVMSG " + data[index].getNickname() + " :PONG\r\n";
	sendMsgUser(index, message);
}


void	Server::cap(clientIt index, std::vector<std::string> &arguments)
{
	//std::cout << "CAP REACHED[" << arguments[1] << "]\n";
	for (uint32_t i = 0; i < CAP_COMMANDS;i++)
	{
		if (commands.cap_cmd[i] == arguments[1])
		{
			(this->*(commands.cap_func[i]))(index, arguments);
		}
	}
}
	

	
	//COMMAND CAP FUNCTIONS
	
void	Server::cap_req(clientIt index, std::vector<std::string> &arguments)
{

	//std::cout << "REQ REACHED\n";
	std::string ack = "CAP * ACK";
	std::string nack = "CAP * NACK";

	//en vez de esto podemos usar una funcion generica  Server::cap_available() que devuelva las capabilities disponibles, osea las que coinciden en un vector 

	for (uint32_t i = 3; i < arguments.size(); i++)
	{
		//std::cout << color::green <<"[" << i << "]"<< arguments[i] << "\n";
		bool found = false;
		//for (uint32_t j = 0; j < COMMANDS;j++)
		//{
		//0	std::cout << "	[" << j << "]"<<commands.cmd[j] << "\n" << color::reset;
			if (arguments[i] == "multi-prefix")
			{
				ack +=  " multi-prefix";
				found = true;
			}
			else
				nack += " " + arguments[i];
		/*}
		if (!found)
		{
		}*/
	}
	sendMsgUser(index, ack);
	std::cout << "SENDED CAP [" << ack << "]\n";
	std::cout << "SENDED NACK [" << nack << "]\n";
	sendMsgUser(index, nack);
	//sendMssgUser(data[(pollfdIt)index].fd, "CAP END")
}

void	Server::cap_ls(clientIt index, std::vector<std::string> &arguments)
{
	(void)arguments;
	//std::cout << "CAP LS REACHED\n";
	//mostrar las capabilities que ofrecemos, 	REPASARRRRRRRRRRRRRRRRRRRRRRRRRRRR
	std::string message = "CAP * LS :multi-prefix sasl ";//sasl account-notify extended-join away-notify chghost userhost-in-names cap-notify server-time message-tags invite-notify batch echo-message account-tag";
	sendMsgUser((clientIt)index, message);
}

void	Server::cap_end(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;	
	
	//mirar que es 001
	std::string message = "001 " + data[index].getNickname() + " :Welcome to the A O I R C server\n";
	sendMsgUser(index, message);
}

void	Server::cap_ack(clientIt index, std::vector<std::string> &arguments)
{
	(void)arguments;
	std::string message = "CAP * ACK : multi-prefix sasl account-notify extended-join away-notify chghost userhost-in-names cap-notify server-time message-tags invite-notify batch echo-message account-tag";
	sendMsgUser(index, message);
	//sendMsgUser(index, "CAP * END");
}
	
	
void	Server::cap_nak(clientIt index, std::vector<std::string> &arguments)
{

	(void)index;
	(void)arguments;
}
		

/*--------------CAPABILITIES NEGOTIATION -------------*/

/*void	Server::cap_available(std::vector<std::string> &arguments)
{
		//va a devolver las capabilities disponibles, osea las que coinciden 

	std::vector<std::string> availables;


		

}*/


/* ------------------------------------------------------------ */
/*							UTILS								*/
/* ------------------------------------------------------------ */


uint32_t	Server::findChannel(const std::string &name) const
{
	for (uint32_t i =0;i < channels.size();i++)
	{
		if (channels[i].getName() == name)
			return i;
	}
	return (0);
}

std::string Server::readTCPInput(int client_fd) {
	char echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;

	memset(echoBuffer, 0, RCVBUFSIZE);
	recvMsgSize = recv(client_fd, echoBuffer, sizeof(echoBuffer), 0);
	if (recvMsgSize == SERVER_FAILURE)
	{
		perror("recv failed, debug here");
		//exit (EXIT_FAILURE);
		return (std::string(nullptr));
	}
	return std::string(echoBuffer, recvMsgSize);
}
/*
uint32_t	Server::findUsername(const std::string &username) const
{
	for (uint32_t i =0;i < data->sizeReg();i++)
	{
		if ((data->Registered()[i].getUsername()) == username)
		{	
			return i;
		}
	}
	return (0);
}

bool	Server::assertClientPassword(uint32_t indexAct, const std::string &password) const
{
	uint32_t indexReg;

	indexReg = findUsername((*data)[indexAct].getUsername());
	if(indexReg == 0)
	{
		std::cout << "server crash\n";
	}
	else
	{
		if ((*data)[indexAct].checkPassword(password))
		{
			return(true);
			
		}
		else
		{
			return (false);
		}
	}
	return (false);
}
*/

#include <locale>
#include <codecvt>


void Server::sendMsgUser(clientIt it, const std::string &str) const
{
	int buffer_size = 65536;
	std::locale::global(std::locale("en_US.UTF-8"));
	setsockopt(data[(pollfdIt)it].fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	//std::string message = "PRIVMSG " + data[it].getUsername() + " : " + str;
	int code;
	std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
	std::wstring wideStr = converter.from_bytes(str);
	std::string utf8Str = converter.to_bytes(wideStr);
	if ((code = send(data[(pollfdIt)it].fd, utf8Str.c_str(), utf8Str.size(), 0)) <= 0)
	{
		std::error_code ec(errno, std::system_category());
		std::cerr << "[fd: " << data[(pollfdIt)it].fd << "] An error ocurred sending the message: " << color::boldwhite << ec.message() << color::reset << "it = " << it << std::endl;
	}
	std::cout << "bytes sent " << code << '\n';
}
/*
bool checkAdmin(Client *client) {
	if (client->getRole() == CL_ROOT)
		return true;
	else
		return false;
}
*/


/* ---------------------------------------------------------------------------------------- */
/*						INIT SETTINGS													*/
/* ---------------------------------------------------------------------------------------- */



void Server::setCommands()
{
	commands.cmd[0]  = NICK;
	commands.cmd[1]  = USER;
	commands.cmd[2]  = JOIN;
	//commands.cmd[3]  = PART;
	commands.cmd[3]  = PRIVMSG;
	//commands.cmd[5]  = NOTICE;
	//commands.cmd[6]  = QUIT;
	//commands.cmd[7]  = TOPIC;
	//commands.cmd[8]  = MODE;
	//commands.cmd[9]  = NAMES;
	//commands.cmd[10] = WHOIS;
	//commands.cmd[11] = KICK;
	commands.cmd[4] = AWAY;
	//commands.cmd[13] = INVITE;
	commands.cmd[5] = PING;
	//commands.cmd[15] = CAP;

	commands.func[0]  = &Server::nick;
	commands.func[1]  = &Server::user;
	commands.func[2]  = &Server::join;
	//commands.func[3]  = &Server::part;
	commands.func[3]  = &Server::privmsg;
	//commands.func[5]  = &Server::notice;
	//commands.func[6]  = &Server::quit;
	//commands.func[7]  = &Server::topic;
	//commands.func[8]  = &Server::mode;
	//commands.func[9]  = &Server::names;
	//commands.func[10] = &Server::whois;
	//commands.func[11] = &Server::kick;
	commands.func[4] = &Server::away;
	//commands.func[13] = &Server::invite;
	commands.func[5] = &Server::ping;
	//commands.func[15] = &Server::cap;

	//commands.cap_cmd[0]  = CAP_REQ;
	//commands.cap_cmd[1]  = CAP_LS;
	//commands.cap_cmd[2]  = CAP_END;
	//commands.cap_cmd[3]  = CAP_ACK;
	//commands.cap_cmd[4]  = CAP_NAK;
//
	//commands.cap_func[0] = &Server::cap_req;
	//commands.cap_func[1] = &Server::cap_ls;
	//commands.cap_func[2] = &Server::cap_end;
	//commands.cap_func[3] = &Server::cap_ack;
	//commands.cap_func[4] = &Server::cap_nak;
}


/* ---------------------------------------------------------------------------------------- */
/*						DEBUG PRINT													*/
/* ---------------------------------------------------------------------------------------- */


void	Server::printServerStatus() const
{
	static std::chrono::steady_clock::time_point last_time = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point now;
	double timeElapsed;

	now = std::chrono::high_resolution_clock::now();
	timeElapsed = std::chrono::duration_cast<std::chrono::duration<double> >(now - last_time).count();
	if (timeElapsed > 1)
	{
		system("clear");
		printIp();
		std::cout << "≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡ 🖥️  SERVER STATUS ℹ️  ≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n";
	
		//std::cout << "🖥️  👥 SERVER ACTIVE USERS: " << data.size() << '\n' << std::endl;

		std::cout << "USERS (waiting to login or register): " << data.size() << '\n';
		for(clientIt i = 0; i < data.size(); i++)
		{
			std::cout << "\t[" << i << "]  username: " << data[i].getUsername() << " nickname: " << data[i].getNickname() << " role: " << data[i].getRole() << "\n";
		}

		//std::cout << "\t\t CHANNELS LIST:" << channels.size() << "\n";
		//for(uint32_t i = 0; i < channels.size(); i++)
		//{
		//	std::cout << "[" << channels[i].getName() << "]" << std::endl;
		//}
		std::cout << "DEBUG LOG (if any)" << std::endl;
		last_time = now;
	}
}