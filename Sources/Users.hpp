# ifndef USERS_HPP
# define USERS_HPP

# include "ft_irc.hpp"

class Users{

    private:

        std :: string fullname;
        std :: string nickname;
        bool password_verification;

    public:

        Users(std :: string fullname, std :: string nickname):fullname(fullname), nickname(nickname){}

        void setPasswordVerification(bool status);
};

# endif