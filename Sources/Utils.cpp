# include "./ft_irc.hpp"

int isnumber(char *s){
    while(*s)
        if (!isdigit(*s++))
            return (1);
    return (0);
}