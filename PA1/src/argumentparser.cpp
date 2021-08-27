#include "argumentparser.h"

ArgumentParser::ArgumentParser(int &argc, char **argv){
  for (int i=1; i < argc; ++i)
    this->tokens.push_back(std::string(argv[i]));
}

ArgumentParser::~ArgumentParser(){

}

std::string ArgumentParser::getCmdOption(const std::string& option){
  std::vector<std::string>::const_iterator itr;
  itr = std::find(this->tokens.begin(), this->tokens.end(), option);
  if (itr != this->tokens.end() && ++itr != this->tokens.end()){
    return *itr;
  }
  static const std::string empty_string("");
  return empty_string;
}

bool ArgumentParser::cmdOptionExists(const std::string &option){
  // If the option (i.e. "-f") exists in tokens then the -f token will be returned to itr, otherwise the last token
  //    in the list will be returned to itr
  auto itr = std::find(this->tokens.begin(), this->tokens.end(), option);

  return (itr != (this->tokens.end()));
}