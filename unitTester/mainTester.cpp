/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainTester.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 17:36:42 by ecamara           #+#    #+#             */
/*   Updated: 2023/08/01 18:11:28 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Checker.hpp"
#include "../include/Server.hpp"
#include <iostream>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <thread>

struct strJoin_t
{
	std::vector<std::string> str_vec;
	std::string string;
	uint32_t index;
};

std::vector<strJoin_t> setVectorsJoin()
{
	std::vector<strJoin_t> cases = {
		{{"first", "first"}, "first first", 0},
		{{"second"}, "second", 0},
		{{"third", "third", "third", "third"}, "third third third third",0},
		{{"forth", "forth", "forth", "forth"}, "forth forth", 2},
		{{"fifth", "fifth", "fifth", "fifth"}, "fifth", 3},
	};
	return cases;
}

std::vector<std::pair<std::string, std::vector<std::string> > > setVectorsSplitSpace()
{
	std::vector<std::pair<std::string, std::vector<std::string> > > cases = {
		{"first first", {"first", "first"}},
		{" second second ", {"second", "second"}},
		{"third",{"third"}},
		{"        forth       ",{"forth"}},
		{"fifth fifth        fifth",{"fifth","fifth","fifth"}},
	};
	return cases;
}

std::vector<std::pair<std::string, std::vector<std::string> > > setVectorsSplitIrcSpace()
{
	std::vector<std::pair<std::string, std::vector<std::string> > > cases = {
		{"first :first first", {"first", "first first"}},
		{": second second ", {" second second "}},
		{"third third third : thirdthird third", {"third", "third", "third", " thirdthird third"}},
		{"        forth       ",{"forth"}},
		{"fifth fifth        fifth",{"fifth","fifth","fifth"}},
		{"sixth sixth        sixth :",{"sixth","sixth","sixth"}},
	};
	return cases;
}

void checkSplit(Checker &checker)
{
	std::cerr << CYAN << "\n  CHECK SPLIT\n" << RESET;
	std::vector<std::pair<std::string, std::vector<std::string> > > items = setVectorsSplitSpace();
	for (auto elem = items.begin(); elem != items.end(); ++elem)
	{
		checker.assertVector(elem->second, utils::split(elem->first, ' '));
	}
}

void checkSplitIrc(Checker &checker)
{
	std::cerr << CYAN << "\n  CHECK SPLIT IRC\n" << RESET;
	std::vector<std::pair<std::string, std::vector<std::string> > > items = setVectorsSplitIrcSpace();
	for (auto elem = items.begin(); elem != items.end(); ++elem)
	{
		checker.assertVector(elem->second, utils::splitIrcPrameters(elem->first, ' '));
	}
}

void checkstrJoin(Checker &checker)
{
	std::cerr << CYAN << "\n  CHECK JOIN\n" << RESET;
	std::vector<strJoin_t> items = setVectorsJoin();
	for (auto elem = items.begin(); elem != items.end(); ++elem)
	{
		checker.assertCheck(elem->string, utils::joinStr(elem->str_vec, elem->index));
	}
}

void checkUtils(Checker &checker)
{
	checkSplit(checker);
	checkSplitIrc(checker);
	checkstrJoin(checker);
}

void	serverCreateInfo(sd::t_serverInput *serverInfo)
{
	int port = 5555; //y de paso chequea si es un int valido
	serverInfo->password = std::string("hola");
	serverInfo->address.sin_family = AF_INET;
	serverInfo->address.sin_addr.s_addr = INADDR_ANY;
	serverInfo->address.sin_port = htons( port ); //mirar htons
}

int main()
{
	std::ofstream nullStream("/dev/null");
	std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
	std::cout.rdbuf(nullStream.rdbuf());

	// Code with print statements
	Checker checker;
	checkUtils(checker);
	//xcheckServer(checker);

	std::cout.rdbuf(oldCoutStreamBuf);
	return (0);
}