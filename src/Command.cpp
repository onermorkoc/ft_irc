# include "../inc/Command.hpp"

Command::Command(std :: string password): password(password) {}

void Command::commandDirector(std::vector<std::string> &msg, int sender_fd){

    User &user = userMap[sender_fd];

    if (msg[0] == "PASS" && msg.size() == 2 && !user.getPasswd())
        pass(msg[1], user);
    
    if (user.getPasswd()){
        if (msg[0] == "NICK" && msg.size() == 2)
            nick(msg[1], user);
        else if (msg[0] == "USER" && msg.size() == 5)
            this->user(msg, user);
    }
    
    if (user.getLogin()){
        if (msg[0] == "JOIN" && msg.size() == 2)
            join(msg[1], user);
        else if (msg[0] == "PRIVMSG" && msg.size() == 3)
            privmsg(msg[1], msg[2], user);
    }
}

void Command::setServerFd(int server_fd){
    this->server_fd = server_fd;
}

void Command::addUser(int newfd){
    userMap[newfd]; // yeni kullanıcıyı mape ekle
    userMap[newfd].setSocket(newfd);
}

void Command::join(std::string name, User &user){
    
    if (name[0] != '#')
        name.insert(0, "#");

    if (channelMap.find(name) == channelMap.end()){ // Channel yoksa
        Channel currentChannel(name, user);
        channelMap[name] = currentChannel;
    }else
        channelMap[name].addUserSocket(user.getSocket());
    user.addChannel(channelMap[name]);
}

void Command::nick(std::string nickname, User &user){
    user.setNickname(nickname);
    if (user.getLogin())
        sendMessage(user, ":irc 001 " + user.getNickname() + " :Welcome to the IRC Network, " + user.getNickname());
}

void Command::pass(std::string pass, User &user){
    if (pass == password)
        user.setPasswd(true);
    else
        sendMessage(user, ":irc  464  Password incorrect");
}

void Command::privmsg(std::string str, std::string msg, User &user){
    
    msg += "\r\n";
    if (str[0] == '#'){ // CHANNEL PRIVMSG
        if (user.findChannel(str)){
            for (std::set<int>::iterator it = channelMap[str].getUsersBegin(); it != channelMap[str].getUsersEnd(); it++){
                int dest_fd = *it;
                if (dest_fd != server_fd && dest_fd != user.getSocket())
                    if (send(dest_fd, msg.data(), msg.size(), 0) == -1)
                        std::cerr << "send error" << std::endl;
            } 
        }
    }else {} // USER PRIVMSG
}

void Command::user(std::vector<std::string> &split, User &user){
    user.setUsername(split[1]);
    if (user.getLogin())
        sendMessage(user, ":irc 001 " + user.getNickname() + " :Welcome to the IRC Network, " + user.getNickname());
}

void Command::sendMessage(User &user, std::string msg){
    msg += "\r\n";
    if (send(user.getSocket(), msg.data(), msg.size(), 0) == -1)
        std::cerr << "send error" << std::endl;
}