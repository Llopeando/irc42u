/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperBlock.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 17:18:47 by ecamara           #+#    #+#             */
/*   Updated: 2023/06/08 17:47:40 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERBLOCK_HPP
#define OPERBLOCK_HPP

#include <unordered_map>
#include <vector>
#include <iostream>
#include "Utils.hpp"

#define PATHFILE "operBlock.txt"

class OperBlock{
	public:
		OperBlock();
		~OperBlock();
		std::string getOperList();
	//	void addOper(std::sting user, std::string pass);
		bool findOper(const std::string &user)const;
		bool checkPass(const std::string &user, const std::string& pass) const;
	private:
		std::unordered_map<std::string, std::string> block;//nick password
};

#endif