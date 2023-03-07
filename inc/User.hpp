/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaysu <yaysu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:18:08 by alyasar           #+#    #+#             */
/*   Updated: 2023/03/05 21:08:44 by yaysu            ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef USER_HPP
# define USER_HPP

# include "./ft_irc.hpp"
# include "./Channel.hpp"

class Channel;

class User{

    private:
    
        int                 socket;
        bool                correctPassword;
        bool                nicknameSet;
        bool                usernameSet;
        std::string         nickname;
        std::string         username;
        std::string         realname;
        std::set<Channel>   channelSet;

    public:

        User(void);
        bool operator<(const User &other) const;

        int         getSocket(void) const;
        bool        getPasswd(void) const;
        bool        getLogin(void) const;
        bool        findChannel(std::string str);
        void        setSocket(int socket);
        void        setPasswd(bool type);
        void        setNickname(std::string nickname);
        void        setUsername(std::string username);
        void        addChannel(const Channel &channel);
        std::string getNickname(void) const;
        std::string getUsername(void) const;
};

# endif