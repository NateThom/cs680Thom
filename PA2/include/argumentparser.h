#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <string>
#include <vector>
#include <algorithm>

class ArgumentParser{
public:
    ArgumentParser(int &argc, char **argv);
    ~ArgumentParser();
    /// @author iain
    std::string getCmdOption(const std::string &option);
    /// @author iain
    bool cmdOptionExists(const std::string &option);
private:
    std::vector <std::string> tokens;
};

#endif //ARG_PARSER_H
