# ifndef COMMAND_HPP
# define COMMAND_HPP

# include "./User.hpp"

class Command{

    private:

        int                             server_fd;
        std :: string                   password;
        std::map<int, User>             userMap;
        std::map<std::string, Channel>  channelMap;

        void pass(std::vector<std::string> &command, User &user);
        void nick(std::vector<std::string> &command, User &user);
        void user(std::vector<std::string> &split, User &user);
        void join(std::string name, User &user);
        void privmsg(std::vector<std::string> &commands, User &user, bool notice);
        void ping(std::string str, User &user);
        void sendMessage(User &user, std::string msg);
        void sendMessage(User &user, std::string channel_name, std::string msg);
        void sendNames(std::string name, User &user);
        void numericReply(int error, User &user, std::string *context = nullptr);

        // Yeni
        void kick(std::string channel_name, std::string nickname, User &user);
        User &nickname_to_user(std :: string nickname);

    public:
    
        Command(std :: string password);

        void commandDirector(std::vector<std::string> &cmd, int sender_fd);
        void setServerFd(int server_fd);
        void addUser(int fd, char *hostname);
};

# endif