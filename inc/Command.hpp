# ifndef COMMAND_HPP
# define COMMAND_HPP

# include "./User.hpp"

class Command{

    private:

        int                             server_fd;
        std :: string                   password;
        std::map<int, User>             userMap;
        std::map<std::string, Channel>  channelMap;

        void join(std::string name, User &user);
        void nick(std::string nickname, User &user);
        void pass(std::string pass, User &user);
        void privmsg(std::string str, std::string msg, User &user);
        void user(std::vector<std::string> &split, User &user);
        void sendMessage(User &user, std::string msg);

    public:
    
        Command(std :: string password);

        void commandDirector(std::vector<std::string> &msg, int sender_fd);
        void setServerFd(int server_fd);
        void addUser(int fd);
};

# endif