# include "../inc/Command.hpp"

Command::Command(std :: string password): password(password) {}

void   Command::commandDirector(std::vector<std::string> &cmd, int sender_fd) {
    
    User &user = userMap[sender_fd];

    if (cmd[0] == "PASS")
        pass(cmd, user);
    
    if (user.getPasswd()){
        if (cmd[0] == "NICK")
            nick(cmd, user);
        else if (cmd[0] == "USER" && cmd.size() >= 5)
            this->user(cmd, user);
    }
    
    if (user.getLogin()){
        if (cmd[0] == "JOIN" && cmd.size() == 2)
            join(cmd[1], user);
        else if (cmd[0] == "PRIVMSG")
            privmsg(cmd, user, false);
        else if (cmd[0] == "NOTICE")
            privmsg(cmd, user, true);
        else if (cmd[0] == "PING" && cmd.size() == 2)
            ping(cmd[1], user);
        else if (cmd[0] == "KICK" && cmd.size() == 3) // /KICK #abc bob123
            kick(cmd[1], cmd[2], user);
    }
}

// nicknameden mapte ki useri bulur yoksa end döndürür.
User &Command::nickname_to_user(std :: string nickname){

    std :: map<int, User>::iterator first = this->userMap.begin();

    while (first != this->userMap.end()){
        if (first->second.getNickname() == nickname)
            return (first->second);
        first++;
    }
    return (first->second); // Bu önemsiz end()->second ile eşdeğer
}

void Command::kick(std::string channel_name, std::string nickname, User &user){

    // kanal olmuyabilir  ==> 441
    // kanal var ama bu işlemi yapan kanal operatorü olmayabilir  => 482
    // kanal operatoru yapıyordur ama atmaya çalıştığı kişi kanalda olmayabilir
    // kanaldan atılır.

    //Not: Command daki userMap => map<user_fd, User>
    //     Commandaki channelMap => map<channel_name, Channel>

    std::map <std::string, Channel>::iterator channel_iterator = channelMap.find(channel_name);

    if (channel_iterator == channelMap.end()){
        // Kanal Yok
    }else{

        Channel channel = channel_iterator->second;

        if (channel.getOperator() != &user){
            // İşlemi yapan kanal operatörü değil.
        }else{

            User user = this->nickname_to_user(nickname);

            if (!channel.checkUser(user.getSocket())){
                // Atmaya çalıştıgı kişi bu kanalda yok
            }else{

                // kişiyi çıkart
                channel.delUserSocket(user.getSocket());
                user.delChannel(channel);

                if (channel.isEmpty()) // Kanalda user kalmadıysa
                    this->channelMap.erase(channel_name); // Boş Kanalı sil
            }
        }
    }

}

void Command::setServerFd(int server_fd){
    this->server_fd = server_fd;
}

void Command::addUser(int newfd, char *hostname){
    userMap[newfd];
    userMap[newfd].setSocket(newfd);
    userMap[newfd].setHostname(hostname);
}

void Command::pass(std::vector<std::string> &command, User &user){

    if (command.size() < 2){
        numericReply(461, user, &(command[0]));
        return;
    }else if (user.getPasswd()){
        numericReply(462, user);
        return;
    }

    if (command[1] == password)
        user.setPasswd(true);
    else
        numericReply(464, user);
}

void Command::nick(std::vector<std::string> &command, User &user){

    int error_value;

    if (command.size() < 2)
        error_value = 1;
    else
        error_value = checkNickname(command[1], userMap);

    if (error_value == 1)
        numericReply(431, user);
    else if (error_value == 2)
        numericReply(432, user, &(command[1]));
    else if (error_value == 3)
        numericReply(433, user, &(command[1]));
    if (error_value != 0)
        return;

    user.setNickname(command[1]);

    if (user.getLogin())
        numericReply(001, user);
}

void Command::sendMessage(User &user, std::string msg){
    msg += "\r\n";
    std::cout << "message send: " + msg;
    if (send(user.getSocket(), msg.data(), msg.size(), 0) == -1)
        std::cerr << "send error" << std::endl;
}

void Command::sendMessage(User &user, std::string channel_name, std::string msg){
    
    msg += "\r\n";
    for (std::set<int>::iterator it = channelMap[channel_name].getUsersBegin(); it != channelMap[channel_name].getUsersEnd(); it++){
        
        int dest_fd = *it;
        if (dest_fd != server_fd && dest_fd != user.getSocket()){
            std::cout << "msg send: " + msg;
            if (send(dest_fd, msg.data(), msg.size(), 0) == -1)
                std::cerr << "send error" << std::endl;
        }
    } 
}

void Command::sendNames(std::string name, User &user){

    std::string buffer = ":127.0.0.1 353 = " + name + " :";
    std::set<int>::iterator begin = channelMap.find(name)->second.getUsersBegin();
    std::set<int>::iterator end = channelMap.find(name)->second.getUsersEnd();
    
    while (begin != end)
        buffer = buffer + " " + (((channelMap.find(name)->second.getOperator()) == &user) ? "@" : "") + userMap[*begin++].getNickname();
    
    sendMessage(user, buffer);
    sendMessage(user, ":127.0.0.1 366 = " + name + " :End of /NAMES list");
}

void Command::join(std::string name, User &user){

    if (name[0] != '#')
        name.insert(0, "#");

    if (channelMap.find(name) == channelMap.end()) { // Channel Yoksa
        Channel newChannel(name, user);
        channelMap[name] = newChannel;
    }else
        channelMap[name].addUserSocket(user.getSocket());

    user.addChannel(channelMap[name]);
    std::string msg = ":" + user.getNickname() + " JOIN :" + name;
    sendMessage(user, name, msg);
    sendNames(name, user);
}

void Command::user(std::vector<std::string> &split, User &user){
    user.setUsername(split[1]);
    if (user.getLogin())
        numericReply(001, user);
}

void Command::privmsg(std::vector<std::string> &command, User &user, bool notice){

    bool user_found = false;

    if (command.size() < 3){
        numericReply(461, user, &(command[0]));
        return;
    }

    if (command[1][0] == '#') { // CHANNEL PRIVMSG

        if (user.findChannel(command[1])){

            std::string msg = user.getSource();
            for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
                msg += (" " + *it);
            sendMessage(user, command[1], msg);

        }else {
            
            if (channelMap.find(command[1]) == channelMap.end())
                if (!notice)
                    numericReply(403, user, &(command[1]));
            else
                numericReply(404, user, &(command[1]));
        }

    } else { // USER PRIVMSG

        for (std::map<int, User>::iterator i = userMap.begin(); i != userMap.end(); i++) {
            
            if (command[1] == i->second.getNickname()) {
                
                user_found = true;
                std::string msg = user.getSource();
                
                for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); it++)
                    msg += (" " + *it);
                sendMessage(i->second, msg);
            }
        }
        if (!user_found && !notice)
            numericReply(401, user, &(command[1]));
    }
}

void Command::ping(std::string str, User &user){
    sendMessage(user, ":irc PONG IRC " + str);
}

void Command::numericReply(int error, User &user, std::string *context) {

    std::string msg = ":irc " + toString(error) + " ";
    if (context != nullptr)
        msg += *context + " ";

    switch (error){

        case 001:
            msg +=  user.getNickname() + " :Welcome to the IRC Network, " + user.getNickname();
            break;
        case 401:
            msg += ":No such nick";
            break;
        case 403:
            msg += ":No such channel";
            break;
        case 404:
            msg += ":Cannot send to channel";
            break;
        case 431:
            msg += ":Nickname is empty";
            break;
        case 432:
            if (context->size() > 20)
                msg += ":Nickname is too long";
            else if (!isalpha((*context)[0]))
                msg += ":Nickname has to begin with a letter";
            else
                msg += ":Nickname must only contain alphanum characters or underscores";
            break;
        case 433:
            msg += ":This nickname is already in use, try another nickname";
            break;
        case 461:
            msg += "Not enough parameters";
            break;
        case 462:
            msg += ":You may not reregister";
            break;
        case 464:
            msg += "Password incorrect";
            break;
        case 441:
			msg += " :They aren't on that channel";
			break;
		case 482:
			msg += " :You're not channel operator";
			break;
    }
    sendMessage(user, msg);
}