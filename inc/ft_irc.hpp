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

// Utils
int error(int key_code);
int isnumber(char *s);
int receiveMsg(int socket, std::string &buff);
std::vector<std::string>    splitMsg(std::string content);

# endif