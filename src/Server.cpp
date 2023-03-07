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
Server::Server(std::string port, std::string pass): fd_count(0), fd_max(5), port(port), command(pass){
    pfds = static_cast<struct pollfd *>(malloc(sizeof(*pfds) * fd_max));
}

// Serveri oluştur
int Server::create(void){
    
    struct addrinfo hints;
    struct addrinfo *ai;
    struct addrinfo *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;
    
    if ((getaddrinfo(NULL, port.data(), &hints, &ai)) != 0)
        return (error(8));

    for (p = ai; p != NULL; p = p->ai_next){

        server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_fd < 0)
            continue;

        if (bind(server_fd, p->ai_addr, p->ai_addrlen) < 0){
            close(server_fd);
            continue;
        }
        break;
    }
    freeaddrinfo(ai);

    if (p == NULL)
        return (error(3));

    if (listen(server_fd, SOMAXCONN) == -1)
        return (error(5));

    command.setServerFd(server_fd);
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

    int newfd = accept(server_fd, (struct sockaddr *)(&clientaddr), &addrlen);
    
    if (newfd == -1)
        error(6);
    else{
        addToPfds(newfd);
        this->command.addUser(newfd);
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
    int data_byte = receiveMsg(pfds[index].fd, buff);
    int sender_fd = pfds[index].fd;
    
    if (data_byte <= 0){

        if (data_byte == 0)
            std::cout << "=> Socket " << sender_fd << " hung up." << std::endl;
        else
            error(9);
        delFromPfds(index);

    }else{
        std::vector<std::string> split_msg = splitMsg(std::string(buff));
        this->command.commandDirector(split_msg, sender_fd);
    }
}