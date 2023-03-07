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

void User::setSocket(int socket){
    this->socket = socket;
}

int User::getSocket(void) const {
    return (socket);
}

bool User::getLogin(void) const {
    return (correctPassword && nicknameSet && usernameSet);
}

void User::setPasswd(bool type){
    correctPassword = type;
}

bool User::getPasswd(void) const {
    return (correctPassword);
}

void User::setNickname(std::string nickname){
    this->nickname = nickname;
    nicknameSet = true;
}

std::string User::getNickname(void) const {
    return (nickname);
}

void User::setUsername(std::string username){
    this->username = username;
    usernameSet = true;
}

std::string User::getUsername(void) const {
    return (username);
}

void User::setHostname(std::string hostname){
    this->hostname = hostname;
}

std::string User::getHostname(void) const {
    return (hostname);
}

bool User::operator<(const User &other) const {
    return (socket < other.socket);
}

void User::addChannel(const Channel &channel){
    channelSet.insert(channel);
}

bool User::findChannel(std::string str){   
    for (std::set<Channel>::iterator it = channelSet.begin(); it != channelSet.end(); it++)
        if ((*it).getName() == str)
            return 1;
    return 0;
}

std::string User::getSource(void) const {
    
    std::string source = ":";
    source += getNickname();
    source += "!";
    source += getUsername();
    source += "@";
    source += getHostname();
    return (source);
}

bool User::hasNickname(void) const {
    return (nicknameSet);
}