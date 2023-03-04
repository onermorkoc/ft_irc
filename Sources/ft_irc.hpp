# ifndef FT_IRC_HPP
# define FT_IRC_HPP

# define MAX_ACCEPT 5

# include <iostream>
# include <sys/socket.h>    // sockect fonksiyonu için
# include <netinet/in.h>    // sockaddr_in structğı için
# include <unistd.h>        // close fonksiyonu için 
# include <sys/select.h>    // select fonksiyonu için
# include <cstring> // memset için

# include "./Server.hpp"
# include "./Users.hpp"

struct data{
    std :: string weechat_recv;
};

int error(int key_code);
int isnumber(char *s);
int breakLoop(int &a, int b);

# endif