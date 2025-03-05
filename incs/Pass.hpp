#ifndef PASS_HPP
#define PASS_HPP

class Pass : public Command
{
     public:
        void execute(Server &serv, Client &client, const std::vector<std::string>& args);
}

#endif