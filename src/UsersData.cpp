/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UsersData.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 16:36:33 by ecamara           #+#    #+#             */
/*   Updated: 2023/04/28 20:36:12 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/UsersData.hpp"

UsersData::UsersData()
{
	//reservar posicion 0 de todos los array como INTOCABLES: cliente SERVER a pollfds[0] ,  activeindex , tmp y a registered , 
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SERVER_FAILURE)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	newTempUser(fd);
	tempClients[0].setUsername("ROOT");
	// ...
	registered.push_back((*this)[0]);
	(*this)[0].setRole(CL_ROOT);
	//password
}

UsersData::~UsersData()
{
	
}

void UsersData::newRegisteredUser(uint32_t indexAct)
{
	uint32_t indexTemp = actives[indexAct].index;
	registered.push_back((*this)[indexAct]);
	actives[indexAct].registered = true;

	if (tempClients[indexTemp].getNewClient())//sign up
		actives[indexAct].index = registered.size() - 1;
	else//logIn
		actives[indexAct].index = findUsername((*this)[indexAct].getUsername());
	popTemp(indexTemp);
}

void UsersData::findTmpIndexAndRest(uint32_t indexTemp, uint32_t num)
{
	for(uint32_t i = 0;i < actives.size();i++)
		if (!actives[i].registered  && actives[i].index == indexTemp)
			actives[i].index -= num;
}

void UsersData::popTemp(uint32_t indexTemp)
{
	if (tempClients.size() > 2 && indexTemp != tempClients.size() - 1)
	{
		tempClients[indexTemp] = tempClients[tempClients.size() - 1];
		findTmpIndexAndRest(tempClients.size() - 1, tempClients.size() - 1 - indexTemp);
	}
	tempClients.pop_back();
}

Client& UsersData::operator[](uint32_t indexAct)
{
	if (actives[indexAct].registered)
		return registered[actives[indexAct].index];
	else
		return tempClients[actives[indexAct].index];
}

const Client& UsersData::operator[](uint32_t indexAct) const
{
	if (actives[indexAct].registered)
		return registered[actives[indexAct].index];
	else
		return tempClients[actives[indexAct].index];
}

void UsersData::newTempUser(int fd)
{
     //añadir a actives
	t_activeIndex newClientIdxAct;
	
	newClientIdxAct.index = static_cast<uint32_t>(tempClients.size());
	newClientIdxAct.registered = false;
	actives.push_back(newClientIdxAct);

    //añadir a pollfd
	struct pollfd newClientFd;

	newClientFd.fd = fd;
	newClientFd.events = POLLOUT | POLLIN;
	pollfds.push_back(newClientFd); 

	//añadir a temp
	tempClients.push_back(Client());

	std::cout << "----------ARRAY ha entrado con bool [" << (*this)[newClientIdxAct.index].getInputBlock()  << "]"<< std::endl;
}

int UsersData::getFd(uint32_t indexAct)const{
	return pollfds[indexAct].fd;
}

/*struct pollfd UsersData::getPollfd(uint32_t indexAct)const
{

	return pollfds[indexAct];
}

struct pollfd *UsersData::getPollfdData()
{
	return pollfds.data();
}*/


std::vector<struct pollfd>&	UsersData::Pollfd(){
	return this->pollfds;
}
		
std::vector<t_activeIndex>&	UsersData::Actives(){
	return this->actives;
}
		
std::vector<Client>&	UsersData::Registered(){
	return this->registered;
}
		
std::vector<Client>&	UsersData::TempClients(){
    return this->tempClients;
}
		

uint32_t	UsersData::sizeAct(){

    return actives.size();
}
		
uint32_t	UsersData::sizeReg(){
    
    return registered.size();
}
		
uint32_t	UsersData::sizeTmp(){
    
    return tempClients.size();
}

uint32_t	UsersData::findUsername(const std::string &username) const
{
	for (uint32_t i =0;i < registered.size();i++)
	{
		if (registered[i].getUsername() == username)
		{	
			return i;
		}
	}
	return (0);
}