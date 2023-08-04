#include "../include/Client.hpp"

/* --- CONSTRUCTORS and DESTRUCTOR --- */

Client::Client() { };

Client::Client(std::string hostName):hostname(hostName), role(CL_USER), authentificied(sd::eNone) {
	state = CL_STATE_INPUT;
	this->isAway = false;
	this->away_msg = "";
	channelOps = 0;
}

Client::Client(const Client &client)
{
	*this = client;
}

Client::~Client() {
	return ;
}

Client& Client::operator=(const Client& client) {
	if (this != &client) {
	this->nickname = client.nickname;
	this->username = client.username;
	this->password = client.password;
	this->role = client.role;
	this->state = client.state;
	this->isAway = client.isAway;
	this->hostname = client.getHostname();
	this->buffer = client.getBuffer();
	this->channelOps = client.getChannelToOps();
	this->authentificied = client.getAuthentificied();
	}
	return *this;
}

void	Client::emptyBuffer()
{
	buffer.clear();
}

/* --- DATA SETTERS --- */

void Client::addBuffer(const std::string &str)
{
	buffer += str; 
}

void Client::setAuthentificied(int status)
{
	authentificied |= status;
}

void Client::setRole(uint8_t role) {
	this->role = role;
}

void	Client::setAwayStatus(bool isAway) {
	this->isAway = isAway;
}

void	Client::setAwayMsg(std::string away_msg) {
	this->away_msg = away_msg;
}

void Client::setUsername(std::string username) {
	this->username = username;
}

void Client::setNickname(std::string nickname) {
	this->nickname = nickname;
}

void Client::setPassword(std::string password) { 
	this->password = password;
}

void	Client::addChannelToOps(sd::channIt index)
{

	channelOps |= (1 << index);
}

/* --- DATA GETTERS --- */

std::string Client::getBuffer() const {
	return buffer;
}

int	Client::getAuthentificied() const {
	return authentificied;
}

uint32_t	Client::getChannelToOps() const {
	return (channelOps);
}

std::string	Client::getUsername() const {
	return (username);
}

std::string	Client::getAwayMsg() const {
	return (away_msg);
}

std::string	Client::getNickname()const{
	return (this->nickname);
}

bool	Client::getAwayStatus() const {
	return (isAway);
}

uint8_t	Client::getState() const {
	return (state);
}

uint32_t	Client::getRole()const {
	return (role);
}

std::string Client::getHostname()const{
	return hostname;
}

std::string Client::getUserMask()const{
	return nickname + "!" + username + "@" + hostname;
}

/* --- CHECKER FUNCTIONS --- */

bool Client::checkPassword(std::string pass)const {

	if (this->password == pass)
		return (true);
	else
		return (false);
}

/* --- FINDERS --- */

bool	Client::findChannelInOps(sd::channIt index) const //con operadores de bits porque eneko mola
{
	return (1 << index) & channelOps;
}

/* UPDATE */
#include "../include/Utils.hpp"

void Client::updateOps(uint32_t channel) {

	channelOps &= ~(1 << channelOps);
	uint32_t mask = (1 << channel) - 2; // -1 to get all 1 and - 2 to set the least fignificant bit to 0
	uint32_t preserved = channelOps & mask;
	uint32_t max = -1;
	uint32_t shifted = (channelOps & (max & ~mask)) >> 1;
	channelOps = shifted | preserved;
	if (channelOps & 1)
		channelOps -= 1;
}
