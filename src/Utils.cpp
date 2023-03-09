# include "../inc/ft_irc.hpp"
# include "../inc/User.hpp"

int isnumber(char *s){
    while(*s)
        if (!isdigit(*s++))
            return (1);
    return (0);
}

bool isCrlf(string str){
    size_t size = str.size();
    return (size >= 2 && str[size - 2] == '\r' && str[size - 1] == '\n');
}

int receiveMsg(int socket, string &buff){
    
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

vector<string> splitMsg(string content) {

    char *words = new char[content.length() + 1];
	strcpy(words, content.c_str());
	char *line = strtok(words, " ");
	vector<string> clientMsg;
	
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

vector<string> splitCommands(string content){

    vector<string> split;
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
            cout << "Error: ./ircserv <port> <password>" << endl;
            break;
        case 2:
            cout << "Error: Invalid port number !" << endl;
            break;
        case 3:
            cout << "Error: socket() failed !" << endl;
            break;
        case 4:
            cout << "Error: bind() failed !" << endl;
            break;
        case 5:
            cout << "Error: listen() failed !" << endl;
            break;
        case 6:
            cout << "Error: accept() failed !" << endl;
            break;
        case 7:
            cout << "Error: poll() failed !" << endl;
            break;
        case 8:
            cout << "Error: getaddrinfo() failed !" << endl;
            break;
        case 9:
            cout << "Error: recv() failed !" << endl;
            break;
        default:
            break;
    }
    return (1);
}

string toString(int n){
    
    if (n == 1)
        return ("001");
    else
        return (std::to_string(n));
}

bool isalnumunderscore(char c){
	return (!(isalnum(c) || c == '_'));
}

int checkNickname(string &nickname, map<int, User> &userMap){

    if (nickname.empty())
        return (1);
    else if (nickname.size() > 20 || !isalpha(nickname[0]) || std::find_if(nickname.begin(), nickname.end(), isalnumunderscore) != nickname.end())
        return (2);
    else{
        for (map<int, User>::iterator it = userMap.begin(); it != userMap.end(); it++)
            if (it->second.getNicknameSet() && it->second.getNickname() == nickname)
                return (3);
    }
    return (0);
}