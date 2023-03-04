# ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "./ft_irc.hpp"

class Channel{

    private:
        std :: string name;
        //std :: string topic;
    public:
        Channel(std :: string name): name(name){}
};

# endif