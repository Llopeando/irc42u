/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:48:56 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/03 16:59:53 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "include/defines.h"

void	printLn(std::string str)
{
	std::cout << str << '\n';
	std::cout << color::boldwhite <<"\t\t " << "hola como estas" << '\n';
	std::cout << color::boldwhite <<"\t\t " << "bien" << '\n';
	std::cout << color::boldwhite <<"\t\t " << "me voy" << '\n';
	std::cout << color::boldcyan <<"\t\t " << str << '\n';
}

int main()
{
	printLn("hola");
	return 0;
}