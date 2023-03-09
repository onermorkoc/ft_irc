# ifndef COMMAND_HPP
# define COMMAND_HPP

# include "./User.hpp"

typedef map<int, User>::iterator            user_iterator;
typedef map<string, Channel>::iterator      channel_iterator;

class Command{

    private:

        int                     server_fd;
        string                  password;
        map<int, User>          userMap;
        map<string, Channel>    channelMap;

        void pass(vector<string> &cmd, User &user);
        void nick(vector<string> &cmd, User &user);
        void user(vector<string> &split, User &user);
        void join(string name, User &user);
        void privmsg(vector<string> &cmds, User &user, bool notice);
        void ping(string str, User &user);
        void sendMessage(User &user, string msg);
        void sendMessage(User &user, string channel_name, string msg);
        void sendNames(string name, User &user);
        void numericReply(int error, User &user, string *context = nullptr);

        // Yeni
        void kick(vector<string> &cmd, User &user);
        user_iterator nickname_to_iterator(string nickname);

    public:
    
        Command(string password);

        void commandDirector(vector<string> &cmd, int sender_fd);
        void setServerFd(int server_fd);
        void addUser(int newfd, char *hostname);

        // Yeni
        void disconnectUser(int user_fd);
};

# endif