#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <stdlib.h>
#include <iostream>
#include "color.h"

class Checker
{
    public:
        Checker(){_test = 0;};
        ~Checker(){};
        template <typename T>
        void assertCheck(T expected, T actual)
        {
            if (expected == actual)
                std::cout << GREEN << "\tSUCCESS" << RESET << std::endl;
            else
                std::cout << RED << "ERROR. Test " << std::to_string(_test) << " has failed. Expected: " << expected << " Actual: " << actual << RESET << std::endl;
            _test++;
        }
        
        template <typename T, typename T2>
        void assertVectorSize(const T& expected, const T2& actual)
        {
            if (expected.size() == actual.size())
                std::cout << GREEN << "\tSUCCESS" << RESET << std::endl;
            else
                std::cout << RED << "ERROR. Test " << std::to_string(_test) << " has failed. Expected size: " << expected.size() << " Actual size: " << actual.size() << RESET << std::endl;
            _test++;
        }

        template <typename T, typename T2>
        void assertVectorCapacity(const T& expected, const T2& actual)
        {
            if (expected.capacity() == actual.capacity())
                std::cout << GREEN << "\tSUCCESS" << RESET << std::endl;
            else
                std::cout << RED << "ERROR. Test " << std::to_string(_test) << " has failed. Expected size: " << expected.size() << " Actual size: " << actual.size() << RESET << std::endl;
            _test++;
        }
        /*
        void assertCheck(bool expected, bool actual);
        void assertCheck(int expected, int actual);
        void assertCheck(unsigned int expected, unsigned int actual);
        void assertCheck(std::string expected, std::string actual);
        void assertCheck(double expected, double actual);*/
    private:
        int _test;

};


#endif