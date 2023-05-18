#include "../include/Client.hpp"

/* --- CONSTRUCTORS and DESTRUCTOR --- */
Client::Client():role(1){
	state = CL_STATE_INPUT;
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
	this->lastMsgIndex = client.lastMsgIndex;
	this->channel = client.channel;
	this->role = client.role;
	this->state = client.state;
	this->isAway = client.isAway;
	this->inputBlock = client.inputBlock;
    }
    return *this;
}



/* --- DATA SETTERS --- */
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

void Client::setPassword(std::string password) { //ya veremos, esto no es muy seguro
	this->password = password;
}

void Client::setNewClient(bool check) {
	newClient = check;
}

/* --- DATA GETTERS --- */
std::string	Client::getUsername() const {
	return (username);
}

std::string	Client::getAwayMsg() const {
	return (away_msg);
}

std::string	Client::getNickname()const{
	return (this->nickname);
}

bool Client::getNewClient() const{
	return (newClient);
}

bool	Client::getAwayStatus() const {
	return (isAway);
}

uint8_t	Client::getState() const {
	return (state);
}

uint32_t	Client::getChannel() const {
	return (channel);
}

uint32_t	Client::getRole()const {
	return (role);
}

/* --- CHECKER FUNCTIONS --- */
bool Client::checkPassword(std::string pass)const {

	if (this->password == pass)
		return (true);
	else
		return (false);
}

uint32_t Client::getLastMsgIdx()const{
	return(lastMsgIndex);
}

bool Client::getInputBlock()const {
	return inputBlock;
}

void Client::addLastMsgIdx(uint32_t num) {
	lastMsgIndex += num;
}

void Client::resetLastMsgIdx(){
	lastMsgIndex = 0;
}

void Client::setLastMsgIdx(uint32_t msgIdx) {
	lastMsgIndex = msgIdx;
}
