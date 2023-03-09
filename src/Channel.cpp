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

Channel::Channel(string name, User &user): name(name), owner(&user){
    userSockets.insert(user.getSocket());
}

bool Channel::operator<(const Channel &other) const{
    return (name < other.name);
}

Channel &Channel::operator=(const Channel &other){
    this->name = other.name;
    this->owner = other.owner;
    this->userSockets = other.userSockets;
    return (*this);
}

void Channel::addUserSocket(int socket){
    this->userSockets.insert(socket);
}

User *Channel::getOwner(void) const{
    return (this->owner);
}

string Channel::getName(void) const{
    return (this->name);
}

set<int>::iterator Channel::getUsersBegin(void){
    return (this->userSockets.begin());
}

set<int>::iterator Channel::getUsersEnd(void){
    return (this->userSockets.end());
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

// Yeni
bool Channel::isEmpty(void){
    return (this->userSockets.empty());
}