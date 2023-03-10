/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaysu <yaysu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:49:10 by alyasar           #+#    #+#             */
/*   Updated: 2023/03/05 21:39:58 by yaysu            ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(void): correctPassword(false), nicknameSet(false), usernameSet(false) {}

bool User::operator<(const User &other) const {
    return (socket < other.socket);
}

int User::getSocket(void) const {
    return (socket);
}

bool User::getCorrectPassword(void) const {
    return (this->correctPassword);
}

bool User::getLogin(void) const {
    return (correctPassword && nicknameSet && usernameSet);
}

bool User::checkChannel(string channel_name){   
    for (set<Channel>::iterator it = channelSet.begin(); it != channelSet.end(); it++)
        if ((*it).getName() == channel_name)
            return (1);
    return (0);
}

bool User::getNicknameSet(void) const {
    return (this->nicknameSet);
}

void User::setSocket(int socket){
    this->socket = socket;
}

void User::setCorrectPassword(bool status){
    this->correctPassword = status;
}

void User::setNickname(string nickname){
    this->nickname = nickname;
    nicknameSet = true;
}

void User::setUsername(string username){
    this->username = username;
    usernameSet = true;
}

void User::addChannel(const Channel &channel){
    channelSet.insert(channel);
}

void User::setHostname(std::string hostname){
    this->hostname = hostname;
}

string User::getNickname(void) const {
    return (this->nickname);
}

string User::getUsername(void) const {
    return (this->username);
}

string User::getHostname(void) const {
    return (this->hostname);
}

string User::getSource(void) const {
    
    string source = ":";
    source += getNickname();
    source += "!";
    source += getUsername();
    source += "@";
    source += getHostname();
    return (source);
}

// Yeni
void User::delChannel(const Channel &channel){
    this->channelSet.erase(channel);
}

// Yeni
set<Channel> &User::getChannelSet(void){
    return (this->channelSet);
}