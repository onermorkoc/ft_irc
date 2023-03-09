/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alyasar <alyasar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:18:19 by alyasar           #+#    #+#             */
/*   Updated: 2023/03/04 17:11:29 by alyasar          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <poll.h>
# include <map>
# include <vector>
# include <set>
# include <cstring>
# include <algorithm>
# include <signal.h>

using std::string;
using std::map;
using std::vector;
using std::set;
using std::cout;
using std::endl;

# include "./User.hpp"

class User;

// Utils
int error(int key_code);
int isnumber(char *s);
int receiveMsg(int socket, string &buff);
int checkNickname(string &nickname, map<int, User> &userMap);
vector<string> splitMsg(string content);
vector<string> splitCommands(string content);
string toString(int n);

# endif