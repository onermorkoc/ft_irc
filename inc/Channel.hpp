/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alyasar <alyasar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:18:25 by alyasar           #+#    #+#             */
/*   Updated: 2023/03/05 22:02:13 by alyasar          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "./User.hpp"

class User;

class Channel{

    private:

        User        *owner;
        string      name;
        set<int>    userSockets;

    public:

        Channel(void);
        Channel(string name, User &user);
        bool        operator<(const Channel &other) const;
        Channel     &operator=(const Channel &other);
        
        void                addUserSocket(int socket);
        User                *getOwner(void) const;
        string              getName(void) const;
        set<int>::iterator  getUsersBegin(void);
        set<int>::iterator  getUsersEnd(void);

        //Yeni
        bool checkUser(int socket);
        void delUserSocket(int socket);
        bool isEmpty(void);
};

# endif