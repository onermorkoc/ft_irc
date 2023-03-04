# include "./ft_irc.hpp"

/* Notlar:
- Önce server oluşturulacak
- Client den gelen bilgilerle users oluşturulacak
- Client den gelen komutları parsellicez
*/

int main(int argc, char **argv){

    // Argüman Kontrolleri.
    if (argc != 3)
        return (error(1));
    else if (isnumber(argv[1]) || atoi(argv[1]) < 0 || atoi(argv[1]) > 65535)
        return (error(2));

    // Serveri oluştur
    Server server(atoi(argv[1]));

    if (server.create())
        return (1);
    std :: cout << "=> Server  oluşturuldu... Port: " << argv[1] << std :: endl;
    
    // İstekleri kabul et
    server.accept_requests();

    return (0);
}