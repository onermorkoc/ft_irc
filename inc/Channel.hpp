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

        User            *m_operator;
        std::string     name;
        std::set<int>   userSockets;

    public:

        Channel(void);
        Channel(std::string name, User &user);
        
        void                        addUserSocket(int socket);
        bool                        operator<(const Channel &other) const;
        User                       *getOperator(void) const;
        Channel                     &operator=(const Channel &other);
        std::string                 getName(void) const;
        std::set<int>::iterator     getUsersBegin(void);
        std::set<int>::iterator     getUsersEnd(void);

        //Yeni
        bool checkUser(int socket);
        void delUserSocket(int socket);
        bool isEmpty(void);
};

# endif