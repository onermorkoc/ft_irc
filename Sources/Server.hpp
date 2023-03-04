# ifndef SERVER_HPP
# define SERVER_HPP

# include "./ft_irc.hpp"

/* Serverin Çalışma mantığı:
1) Soket oluşturulur (socket)
2) Soketden dönen fd' ile ip ve  port ilişkilendirilir (bind)
3) Sunucu dinlenmeye başlanır (listen)
4) Sunucuya gelen istekler kabul edilir (accecpt). Loop burada olur.
5) Clientten gelen veriler alınır ve gönderilir (recv/send)
*/

class Server{
    
    private:

        int port;
        int server_fd;
        int client_fd;
        sockaddr_in server_info;
        sockaddr_in client_info;

    public:

        Server(int port): port(port){};

        int create(void);
        int accept_requests(void);
};

# endif