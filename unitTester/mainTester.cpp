/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainTester.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 17:36:42 by ecamara           #+#    #+#             */
/*   Updated: 2023/03/10 18:47:41 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Checker.hpp"
#include <iostream>

int ft_prueba(const char *s, int num)
{
	return(atoi(s) + num);
};

int main()
{
	Checker checker;

	checker.assertCheck(100, ft_prueba("50" , 50));
	checker.assertCheck(100, ft_prueba("49" , 50));


	return (0);
}