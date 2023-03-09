/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alyasar <alyasar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:18:01 by alyasar           #+#    #+#             */
/*   Updated: 2023/03/04 16:18:02 by alyasar          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

/* Serverin Çalışma mantığı:
1) Soket oluşturulur (socket)
2) Soketden dönen fd' ile ip ve  port ilişkilendirilir (bind)
3) Sunucu dinlenmeye başlanır (listen)
4) Sunucuya gelen istekler kabul edilir (accecpt). Loop burada olur.
5) Clientten gelen veriler alınır ve gönderilir (recv/send)
*/

# include "../inc/Server.hpp"

int main(int argc, char **argv){
    
    // Argüman Kontrolleri.
    if (argc != 3)
        return (error(1));
    else if (isnumber(argv[1]) || atoi(argv[1]) < 0 || atoi(argv[1]) > 65535)
        return (error(2));

    // Server oluştur ve çalıştır.
    Server server(argv[1], argv[2]);

    if (server.create())
        return (1);
        
    cout << "=> Server created... Port: " << argv[1] << " Password: " << argv[2] << endl;
    server.run();

    return (0);
}