/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 12:28:43 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/24 12:18:39 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define FLAG_ID            0b00000001
#define FLAG_ENTITY_VECTOR 0b00000010
#define FLAG_PLAYER_VECTOR 0b00000100
#define FLAG_NEW_ENTITY    0b00001000
#define FLAG_NEW_POSITION  0b00010000

#include <iostream>

#include "Color.hpp"
#include "../glm/glm.hpp"

struct Info{
	uint32_t flags;
	uint32_t size;
};

struct EntityData{
	uint32_t id;
	uint32_t flags;
};

struct Entity{
	glm::vec2 pos;
	glm::vec2 vel;
	uint64_t data;
};

struct userIt{
	uint32_t index;
	userIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	userIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	userIt& operator++() {
		++index;
		return *this;
	}
};

struct pollfdIt{
	uint32_t index;
	pollfdIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	pollfdIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	pollfdIt& operator++() {
		++index;
		return *this;
	}
	pollfdIt operator++(int) {
		pollfdIt tmp(*this);
		++index;
		return tmp;
	}
};

struct posIt{
	uint32_t index;
	posIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	posIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	posIt& operator++() {
		++index;
		return *this;
	}
	posIt operator++(int) {
		posIt tmp(*this);
		++index;
		return tmp;
	}
};

struct idIt{
	uint32_t index;
	idIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	idIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	idIt& operator++() {
		++index;
		return *this;
	}
	idIt operator++(int) {
		idIt tmp(*this);
		++index;
		return tmp;
	}
};

struct flagsIt{
	uint32_t index;
	flagsIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	flagsIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	flagsIt& operator++() {
		++index;
		return *this;
	}
	flagsIt operator++(int) {
		flagsIt tmp(*this);
		++index;
		return tmp;
	}
};

struct velIt{
	uint32_t index;
	velIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	velIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	velIt& operator++() {
		++index;
		return *this;
	}
	velIt operator++(int) {
		velIt tmp(*this);
		++index;
		return tmp;
	}
};

struct entityIt{
	uint32_t index;
	entityIt(int value = 0):index(value){};
	operator uint32_t() const {
		return index;
	}
	entityIt& operator=(uint32_t value){
		index = value;
		return *this;
	}
	entityIt& operator++() {
		++index;
		return *this;
	}
	entityIt operator++(int) {
		entityIt tmp(*this);
		++index;
		return tmp;
	}
};
