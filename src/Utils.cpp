# include "../inc/ft_irc.hpp"
# include "../inc/User.hpp"

int isnumber(char *s){
    while(*s)
        if (!isdigit(*s++))
            return (1);
    return (0);
}

bool isCrlf(std::string str){
    std::string::size_type size = str.size();
    return (size >= 2 && str[size - 2] == '\r' && str[size - 1] == '\n');
}

int receiveMsg(int socket, std::string &buff){
    
    buff.clear();
    char lineRead[4096];
    memset(lineRead, 0, 4096);
    int rd;

    while ((rd = recv(socket, lineRead, 4096, 0)) != -1){
        buff += lineRead;
        if (lineRead[rd] == '\0' || isCrlf(buff))
            return (buff.size());
        memset(lineRead, 0, 4096);
    }
    return (rd);
}

std::vector<std::string> splitMsg(std::string content) {

    char *words = new char[content.length() + 1];
	std::strcpy(words, content.c_str());
	char *line = strtok(words, " ");
	std::vector<std::string> clientMsg;
	
	while(line != NULL){
		clientMsg.push_back(line);
		line = strtok(NULL, "\r \n");
	}
	delete[] words;

    if (clientMsg.size() == 1 && clientMsg[0].length() > 1 && clientMsg[0][clientMsg[0].length() - 1] == '\n'\
         && clientMsg[0][clientMsg[0].length() - 2] == '\r')
            clientMsg[0].erase(clientMsg[0].length() - 2, 2); // Sondaki \r\n siliyorum
	return (clientMsg);
}

std::vector<std::string> splitCommands(std::string content){

    std::vector<std::string> split;
    int end = content.find("\r\n");
    
    while (end != -1){
        split.push_back(content.substr(0, end + 2));
        content.erase(0, end + 2);
        end = content.find("\r\n");
    }
    return (split);
}

int error(int key_code){

    switch(key_code){

        case 1:
            std :: cout << "Error: ./ircserv <port> <password>" << std :: endl;
            break;
        case 2:
            std :: cout << "Error: Invalid port number !" << std :: endl;
            break;
        case 3:
            std :: cout << "Error: socket() failed !" << std :: endl;
            break;
        case 4:
            std :: cout << "Error: bind() failed !" << std :: endl;
            break;
        case 5:
            std :: cout << "Error: listen() failed !" << std :: endl;
            break;
        case 6:
            std :: cout << "Error: accept() failed !" << std :: endl;
            break;
        case 7:
            std :: cout << "Error: poll() failed !" << std :: endl;
            break;
        case 8:
            std :: cout << "Error: getaddrinfo() failed !" << std :: endl;
            break;
        case 9:
            std :: cout << "Error: recv() failed !" << std :: endl;
            break;
        default:
            break;
    }

    return (1);
}

std::string toString(int n){
    
    if (n == 1)
        return ("001");
    else
        return (std::to_string(n));
}

bool isalnumunderscore(char c){
	return (!(isalnum(c) || c == '_'));
}

int checkNickname(std::string &nickname, std::map<int, User> &userMap){

    if (nickname.empty())
        return (1);
    else if (nickname.size() > 20 || !isalpha(nickname[0]) || std::find_if(nickname.begin(), nickname.end(), isalnumunderscore) != nickname.end())
        return (2);
    else{
        for (std::map<int, User>::iterator it = userMap.begin(); it != userMap.end(); it++)
            if (it->second.hasNickname() && it->second.getNickname() == nickname)
                return (3);
    }
    return (0);
}