# include "./ft_irc.hpp"

int error(int key_code){

    switch(key_code){

        case 1:
            std :: cout << "Hata: ./ircserv <port> <password>" << std :: endl;
            break;
        case 2:
            std :: cout << "Hata: Geçersiz port numarası !" << std :: endl;
            break;
        case 3:
            std :: cout << "Hata: socket() işlevini yerine getiremedi !" << std :: endl;
            break;
        case 4:
            std :: cout << "Hata: bind() işlevini yerine getiremedi !" << std :: endl;
            break;
        case 5:
            std :: cout << "Hata: listen() işlevini yerine getiremedi !" << std :: endl;
            break;
        case 6:
            std :: cout << "Hata: accept() işlevini yerine getiremedi !" << std :: endl;
            break;
        case 7:
            std :: cout << "Hata: select() işlevini yerine getiremedi !" << std :: endl;
            break;
        default:
            break;
    }

    return (1);
}