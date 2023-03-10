/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checker.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:21:00 by ecamara           #+#    #+#             */
/*   Updated: 2022/12/31 18:31:53 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <stdlib.h>
#include <iostream>
#include "color.h"

class Checker
{
    public:
        Checker();
        ~Checker();
        void assertCheck(bool expected, bool actual);
        void assertCheck(int expected, int actual);
        void assertCheck(unsigned int expected, unsigned int actual);
        void assertCheck(std::string expected, std::string actual);
        void assertCheck(double expected, double actual);
    private:
        int _test;

};

#endif