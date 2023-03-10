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
    
        int             socket;
        bool            correctPassword;
        bool            nicknameSet;
        bool            usernameSet;
        string          nickname;
        string          username;
        string          realname;
        string          hostname;
        set<Channel>    channelSet;

    public:

        User(void);
        bool    operator<(const User &other) const;

        int         getSocket(void) const;
        bool        getCorrectPassword(void) const;
        bool        getLogin(void) const;
        bool        checkChannel(string channel_name);
        bool        getNicknameSet(void) const;
        void        setSocket(int socket);
        void        setCorrectPassword(bool status);
        void        setNickname(string nickname);
        void        setUsername(string username);
        void        addChannel(const Channel &channel);
        void        setHostname(string hostname);
        string      getNickname(void) const;
        string      getUsername(void) const;
        string      getHostname(void) const;
        string      getSource(void) const;
        
        // Yeni
        void delChannel(const Channel &channel);
        set<Channel> &getChannelSet(void);
};
# endif