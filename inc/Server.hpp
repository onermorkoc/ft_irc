/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alyasar <alyasar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:18:12 by alyasar           #+#    #+#             */
/*   Updated: 2023/03/05 19:12:04 by alyasar          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

# include "./ft_irc.hpp"
# include "./Command.hpp"

class Server {

    private:

        int                 server_fd;
        int                 fd_count;
        int                 fd_max;
        Command             command;
        std :: string       port;
        struct pollfd       *pfds;

        void newConnection(void);
        void newMessage(int index);
        void addToPfds(int newfd);
        void delFromPfds(int index);
        
    public:

        Server(std::string port, std::string pass);

        // Yeni
        ~Server(void);

        int create(void);
        int run(void);
};

# endif