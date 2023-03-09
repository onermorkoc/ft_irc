# include "../inc/Command.hpp"

Command::Command(string password): password(password) {}

void Command::commandDirector(vector<string> &cmd, int sender_fd) {
    
    User &user = userMap[sender_fd];

    if (cmd[0] == "PASS")
        pass(cmd, user);
    
    if (user.getCorrectPassword()){
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
        else if (cmd[0] == "KICK" /*&& cmd.size() == 3*/) // /KICK #abc bob123
            kick(cmd, user);
    }
}

user_iterator Command::nickname_to_iterator(string nickname){

    user_iterator first = this->userMap.begin();

    while (first != this->userMap.end()){
        if (first->second.getNickname() == nickname)
            return (first);
        first++;
    }
    return (first); // Bu end() ile eşdeğer
}
    
void Command::kick(vector<string> &cmd, User &user){

    channel_iterator c_ite = channelMap.find(cmd[1]);

    if (c_ite == channelMap.end()){ // Kanal Yok
        // numericReply(); => 403
        return;
    }else{

        Channel channel = c_ite->second;

        if (channel.getOwner() != &user){ // İşlemi yapan kanal operatörü değil.
            // numericReply(); => 482
            return;
        }else{

            user_iterator kicked_ite = nickname_to_iterator(cmd[2]);

            if (kicked_ite == userMap.end()){ // Atmaya çalıştığı kişi server de yok
                // numericReply();
                return;
            }else{

                User kicked = kicked_ite->second;

                if (!channel.checkUser(kicked.getSocket())){ // Atmaya çalıştıgı kişi bu kanalda yok
                    // numericReply(); => 441
                    return;
                }else{

                    // kişiyi çıkart
                    channel.delUserSocket(kicked.getSocket());
                    kicked.delChannel(channel);

                    if (channel.isEmpty()) // Kanalda user kalmadıysa
                        this->channelMap.erase(cmd[1]); // Boş Kanalı sil
                }
            }
        }
    }
}

void Command::disconnectUser(int user_fd){
    // Eksik yapılacak
    cout << "=> Socket " << user_fd << " hung up." << endl;
}

void Command::setServerFd(int server_fd){
    this->server_fd = server_fd;
}

void Command::addUser(int newfd, char *hostname){
    userMap[newfd];
    userMap[newfd].setSocket(newfd);
    userMap[newfd].setHostname(hostname);
}

void Command::pass(vector<string> &cmd, User &user){

    if (cmd.size() < 2){
        numericReply(461, user, &(cmd[0]));
        return;
    }else if (user.getCorrectPassword()){
        numericReply(462, user);
        return;
    }

    if (cmd[1] == password)
        user.setCorrectPassword(true);
    else
        numericReply(464, user);
}

void Command::nick(vector<string> &cmd, User &user){

    int error_value;

    if (cmd.size() < 2)
        error_value = 1;
    else
        error_value = checkNickname(cmd[1], userMap);

    if (error_value == 1)
        numericReply(431, user);
    else if (error_value == 2)
        numericReply(432, user, &(cmd[1]));
    else if (error_value == 3)
        numericReply(433, user, &(cmd[1]));
    if (error_value != 0)
        return;

    user.setNickname(cmd[1]);

    if (user.getLogin())
        numericReply(001, user);
}

void Command::sendMessage(User &user, string msg){
    msg += "\r\n";
    cout << "message send: " + msg;
    if (send(user.getSocket(), msg.data(), msg.size(), 0) == -1)
        cout << "send error" << endl;
}

void Command::sendMessage(User &user, string channel_name, string msg){
    
    msg += "\r\n";
    for (set<int>::iterator it = channelMap[channel_name].getUsersBegin(); it != channelMap[channel_name].getUsersEnd(); it++){
        
        int dest_fd = *it;
        if (dest_fd != server_fd && dest_fd != user.getSocket()){
            cout << "message send: " + msg;
            if (send(dest_fd, msg.data(), msg.size(), 0) == -1)
                cout << "send error" << endl;
        }
    } 
}

void Command::sendNames(string name, User &user){

    string buffer = ":127.0.0.1 353 = " + name + " :";
    set<int>::iterator begin = channelMap.find(name)->second.getUsersBegin();
    set<int>::iterator end = channelMap.find(name)->second.getUsersEnd();
    
    while (begin != end)
        buffer = buffer + " " + (((channelMap.find(name)->second.getOwner()) == &user) ? "@" : "") + userMap[*begin++].getNickname();
    
    sendMessage(user, buffer);
    sendMessage(user, ":127.0.0.1 366 = " + name + " :End of /NAMES list");
}

void Command::join(string name, User &user){

    if (name[0] != '#')
        name.insert(0, "#");

    if (channelMap.find(name) == channelMap.end()) { // Channel Yoksa
        Channel newChannel(name, user);
        channelMap[name] = newChannel;
    }else
        channelMap[name].addUserSocket(user.getSocket());

    user.addChannel(channelMap[name]);
    string msg = ":" + user.getNickname() + " JOIN :" + name;
    sendMessage(user, name, msg);
    sendNames(name, user);
}

void Command::user(vector<string> &split, User &user){
    user.setUsername(split[1]);
    if (user.getLogin())
        numericReply(001, user);
}

void Command::privmsg(vector<string> &cmd, User &user, bool notice){

    bool user_found = false;

    if (cmd.size() < 3){
        numericReply(461, user, &(cmd[0]));
        return;
    }

    if (cmd[1][0] == '#') { // CHANNEL PRIVMSG

        if (user.checkChannel(cmd[1])){

            string msg = user.getSource();
            for (vector<string>::iterator it = cmd.begin(); it != cmd.end(); it++)
                msg += (" " + *it);
            sendMessage(user, cmd[1], msg);

        }else {
            
            if (channelMap.find(cmd[1]) == channelMap.end())
                if (!notice)
                    numericReply(403, user, &(cmd[1]));
            else
                numericReply(404, user, &(cmd[1]));
        }

    } else { // USER PRIVMSG

        for (user_iterator i = userMap.begin(); i != userMap.end(); i++) {
            
            if (cmd[1] == i->second.getNickname()) {
                
                user_found = true;
                string msg = user.getSource();
                
                for (vector<string>::iterator it = cmd.begin(); it != cmd.end(); it++)
                    msg += (" " + *it);
                sendMessage(i->second, msg);
            }
        }
        if (!user_found && !notice)
            numericReply(401, user, &(cmd[1]));
    }
}

void Command::ping(string str, User &user){
    sendMessage(user, ":irc PONG IRC " + str);
}

void Command::numericReply(int error, User &user, string *context) {

    string msg = ":irc " + toString(error) + " ";
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