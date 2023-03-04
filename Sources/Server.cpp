# include "./Server.hpp"

int Server::create(void){
    //                          ipv4       TCP                        
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
        return (error(3));
    
    server_info.sin_addr.s_addr = INADDR_ANY;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(this->port);

    if (bind(server_fd, (const sockaddr*)&server_info, sizeof(server_info)) == -1){
        close(server_fd);
        return (error(4));
    }

    if (listen(server_fd, MAX_ACCEPT) == -1){
        close(server_fd);
        return (error(5));
    }
    return (0);
}

int Server::accept_requests(void){

    int         fd_max;
    fd_set      fd_array;
    fd_set      temp;

    int data_byte;
    char message[256];

    FD_ZERO(&fd_array);         // arraymın içini sıfırlıyorum.
    FD_SET(server_fd, &fd_array);   // server_fd yi arrayımın içine koyuyorum
    fd_max = server_fd;     // mantıkken en son açılan fd, server fd olduğundan eşitliyorum

    while(true){

        temp = fd_array;

        if (select(fd_max + 1, &temp, NULL, NULL, NULL) == -1){
            close(server_fd);
            return (error(7));
        }else{

            for (int i = 0; i <= fd_max; i++){ // açık olan tüm fd leri dolaşıyorum

                if(FD_ISSET(i, &temp)){ // eger i fd si benim arrayın içinde varsa içine gir işlem yapıcam

                    if (i == server_fd){ // eger i server_Fd eşitse o fd nin görevi yeni kullanıcıları eklemesi

                        socklen_t length = sizeof(client_info);
                        client_fd = accept(server_fd, (sockaddr *)&client_info, &length); // bağlantı kabul et

                        if (client_fd < 0){
                            error(6);
                        }else{
                            
                            std :: cout << "=> Servere biri bağlandı ! Soket numarası: " << client_fd << std :: endl;
                            
                            FD_SET(client_fd, &fd_array); // gelen client fd yi fd_arraye ekle
                            
                            if (client_fd > fd_max) // max fd yi editle
                                fd_max = client_fd;
                            

                            //Users new_user()

                        }
                    }else{
                        /* // veri alıp gönder
                        // bağlantı koparan fd leri tespit et
                        // veriyi al herkese yolla (aynı kanallarda olup olmadıgı falan burada olacak)
                        memset(message, 0, 256);
                        if (data_byte = recv(i, message, 256, 0) <= 0){
                            if (data_byte == 0){
                                std :: cout << "Kişi bağlantıyı kapattı" << std :: endl;
                            }else{
                                // başka hata var
                            }
                            
                            close(i);
                            FD_CLR(i, &fd_array);
                        }else{
                            //for (int j = 3; j < fd_max; j++){
                                //std :: cout << "Döndü" << std :: endl;
                                //if (FD_ISSET(j, &fd_array)){
                                    //std :: cout << "Yolladı" << std :: endl;
                                    //send(j, message, data_byte, 0);
                                //}
                            //}
                        } */
                    }
                }
            }
        }
    }
    return  (0);
}