/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checker.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:25:24 by ecamara           #+#    #+#             */
/*   Updated: 2022/12/31 18:50:10 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Checker.hpp"
#include <cmath>

Checker::Checker()
{
    _test = 0;
    std::cout << GREEN << "Checker standar constructor called" << RESET << std::endl;
}

Checker::~Checker()
{
    std::cout << RED << "Checker standar destructor called" << RESET << std::endl;
}

void Checker::assertCheck(int expected, int actual)
{
    if (expected == actual)
        std::cout << GREEN << "\tSUCCESS" << RESET << std::endl;
    else
        std::cout << RED << "ERROR. Test " << std::to_string(_test) << " has failed. Expected: " << expected << " Actual: " << actual << RESET << std::endl;
    _test++;
}

void Checker::assertCheck(unsigned int expected, unsigned int actual)
{
    if (expected == actual)
        std::cout << GREEN << "\tSUCCESS" << RESET << std::endl;
    else
        std::cout << RED << "ERROR. Test "<< std::to_string(_test) << " has failed. Expected: " << expected << " Actual: " << actual << RESET << std::endl;
    _test++;
}


void Checker::assertCheck(std::string expected, std::string actual)
{
    if (expected == actual)
        std::cout << GREEN << "\tSUCCESS" << RESET << std::endl;
    else
        std::cout << RED << "ERROR. Test " << std::to_string(_test) << " has failed. Expected: " << expected << " Actual: " << actual << RESET << std::endl;
    _test++;
}

void Checker::assertCheck(double expected, double actual)
{
    if (expected == actual)
        std::cout << GREEN << "\tSUCCESS" << RESET << std::endl;
    else
        std::cout << RED << "ERROR. Test " << std::to_string(_test) << " has failed. Expected: " << expected << " Actual: " << actual << RESET << std::endl;
    _test++;
}

void Checker::assertCheck(bool expected, bool actual)
{
    if (expected == actual)
        std::cout << GREEN << "\tSUCCESS" << RESET << std::endl;
    else
        std::cout << RED << "ERROR. Test " << std::to_string(_test) << " has failed. Expected: " << expected << " Actual: " << actual << RESET << std::endl;
    _test++;
}