/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alyasar <alyasar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:17:51 by alyasar           #+#    #+#             */
/*   Updated: 2023/03/05 22:07:25 by alyasar          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

// Constructor
Server::Server(std::string port, std::string pass): fd_count(0), port(port), command(pass) {}

// Yeni
Server::~Server(void){
    free(this->pfds);
}

// Serveri oluştur
int Server::create(void){
    
    struct addrinfo     hints;
    struct addrinfo     *ai;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;
    
    if ((getaddrinfo(NULL, port.c_str(), &hints, &ai)) != 0)
        return (error(8));

    server_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (server_fd < 0){
        freeaddrinfo(ai);
        return (error(3));
    }

    if (bind(server_fd, ai->ai_addr, ai->ai_addrlen) < 0){
        close(server_fd);
        freeaddrinfo(ai);
        return (error(4));
    }

    if (listen(server_fd, SOMAXCONN) == -1){
        close(server_fd);
        freeaddrinfo(ai);
        return (error(5));
    }

    freeaddrinfo(ai);
    command.setServerFd(server_fd);
    this->fd_max = server_fd;
    this->pfds = static_cast<struct pollfd *>(malloc(sizeof(*pfds) * fd_max));
    pfds[0].fd = server_fd;
    pfds[0].events = POLLIN;
    fd_count++;
    return (0);
}

// Serveri çalıştır (loop).
int Server::run(void){
    
    while (true){
        
        if (poll(pfds, fd_count, -1) == -1)
            return (error(7));

        for (int i = 0; i < fd_count; i++){
            if (pfds[i].revents & POLLIN){
                if (pfds[i].fd == server_fd)
                    newConnection();
                else
                    newMessage(i);
            }
        }
    }
}

// Yeni kullanıcıları kabul et ve gerekli ayarlamaları yap.
void Server::newConnection(void){

    char clientIP[INET_ADDRSTRLEN];
    struct sockaddr_storage clientaddr;
    socklen_t addrlen = sizeof(clientaddr);

    char hostname[NI_MAXHOST];
    char service[NI_MAXSERV];
    memset(hostname, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    int newfd = accept(server_fd, (struct sockaddr *)(&clientaddr), &addrlen);
    
    if (newfd == -1)
        error(6);
    else{
        addToPfds(newfd);
        getnameinfo((sockaddr *)(&clientaddr), addrlen, hostname, NI_MAXHOST, service, NI_MAXSERV, 0);
        this->command.addUser(newfd, hostname);
        std::cout << "=> New connection from " << inet_ntop(clientaddr.ss_family, &((struct sockaddr_in *)(&clientaddr))->sin_addr, \
            clientIP, INET_ADDRSTRLEN) << " on socket " << newfd << std::endl;
    }
}

// Çoklu fd kontrolü(I/O) için pdfs arrayını düzenle.
void Server::addToPfds(int newfd){

    if (fd_count >= fd_max){
        fd_max *= 2;
        pfds = (struct pollfd *)realloc(pfds, sizeof(*pfds) * fd_max);
    }

    pfds[fd_count].fd = newfd;
    pfds[fd_count].events = POLLIN;
    fd_count++;
}

// Bağlantıyı koparan clients fd lerini tekrar kullanabilmesi için pfds den çıkart
void Server::delFromPfds(int index){
    close(pfds[index].fd);
    pfds[index] = pfds[fd_count - 1];
    fd_count--;
}

// Veriyi al işle veya bağlantıyı koparanları tespit et
void Server::newMessage(int index){

    std::string buff;
    int sender_fd = pfds[index].fd;
    int data_byte = receiveMsg(sender_fd, buff);
    
    if (data_byte <= 0){

        if (data_byte == 0)
            std::cout << "=> Socket " << sender_fd << " hung up." << std::endl;
        else
            error(9);
        delFromPfds(index);

    }else{

        if (buff[buff.length() - 2] != '\r')
            buff.insert(buff.length() - 1, "\r");

        std::vector<std::string> split_commands = splitCommands(buff);

        for (int i = 0; i < split_commands.size(); i++){
            std::cout << "message taken: " << split_commands[i];
            std::vector<std::string> split_msg = splitMsg(split_commands[i]);
            this->command.commandDirector(split_msg, sender_fd);
        }
    }
}