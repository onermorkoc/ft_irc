/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alyasar <alyasar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 18:12:04 by alyasar           #+#    #+#             */
/*   Updated: 2023/03/05 22:00:57 by alyasar          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(void) {}

Channel::Channel(std::string name, User &user): name(name), m_operator(&user){
    userSockets.insert(user.getSocket());
}

bool Channel::operator<(const Channel &other) const{
    return (name < other.name);
}

Channel &Channel::operator=(const Channel &other){
    name = other.name;
    m_operator = other.m_operator;
    userSockets = other.userSockets;
    return (*this);
}

std::set<int>::iterator Channel::getUsersBegin(void){
    return (userSockets.begin());
}

std::set<int>::iterator Channel::getUsersEnd(void){
    return (userSockets.end());
}

std::string Channel::getName(void) const{
    return (name);
}

void Channel::addUserSocket(int socket){
    userSockets.insert(socket);
}

User *Channel::getOperator(void) const {
    return (m_operator);
}

// Yeni
bool Channel::checkUser(int socket){
    if (this->userSockets.find(socket) == this->userSockets.end())
        return (false);
    return (true);
}

// Yeni
void Channel::delUserSocket(int socket){
    this->userSockets.erase(socket);
}