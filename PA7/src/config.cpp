#include "config.h"

Configuration::Configuration() {
  stellar_body_count = 0;

  v_shader_file_path = "";
  f_shader_file_path = "";

  window_name = "";
  window_height = 0;
  window_width = 0;
}

Configuration::~Configuration()
{

}

std::vector<std::string> Configuration::Split(std::string str)
{
  std::vector<std::string> words{};
  std::string word = " ";
  for(char s : str){
    if(s == ' '){
      words.push_back(word);
      word = "";
    }
    else{
      word = word + s;
    }
  }
  words.push_back(word);
  return words;
}

bool Configuration::LoadConfigFile(std::string f_path)
{
  std::string str = "";
  std::ifstream m_file(f_path);
  if(!m_file.is_open()){
    printf("Could not open config file\n");
    return false;
  }
  
  StellarBody* stellarBodyP;

  while(getline(m_file, str)){
    if(str.substr(0,18) == "stellar_body_count"){
      std::vector<std::string> line = Split(str);
      stellar_body_count = stoi(line[1]);
      std::cout << "";
    }
    if(str.substr(0,18) == "v_shader_file_path"){
      std::vector<std::string> line = Split(str);
      v_shader_file_path = line[1];
      std::cout << "";
    }
    if(str.substr(0,18) == "f_shader_file_path"){
      std::vector<std::string> line = Split(str);
      f_shader_file_path = line[1];
      std::cout << "";
    }
    if(str.substr(0,11) == "window_name"){
      std::vector<std::string> line = Split(str);
      window_name = line[1];
      std::cout << "";
    }
    if(str.substr(0,13) == "window_height"){
      std::vector<std::string> line = Split(str);
      window_height = stoi(line[1]);
      std::cout << "";
    }
    if(str.substr(0,12) == "window_width"){
      std::vector<std::string> line = Split(str);
      window_width = std::stoi(line[1]);
      std::cout << "";
    }
    if(str.substr(0,11) == "StellarBody"){
      std::vector<std::string> line = Split(str);

      stellarBodyP = new StellarBody();
      stellarBodyP->name = line[1];
      stellarBodyP->type = line[2];
      stellarBodyP->path_to_obj = line[3];
      stellarBodyP->path_to_mtl = line[4];
      stellarBodyP->aphelion = std::stof(line[5]);
      stellarBodyP->periphelion = std::stof(line[6]);
      stellarBodyP->axial_tilt = std::stof(line[7]);
      stellarBodyP->rotational_period = std::stof(line[8]);
      stellarBodyP->orbital_speed = std::stof(line[9]);

      stellar_list.push_back(*stellarBodyP);
      std::cout << "";
    }
  }
  
  return true;
}

int Configuration::GetStellarBodyCount() const{
  return stellar_body_count;
}

std::string Configuration::GetVShaderFilePath() const{
  return v_shader_file_path;
}

std::string Configuration::GetFShaderFilePath() const{
  return f_shader_file_path;
}

std::string Configuration::GetWindowName() const{
  return window_name;
}

int Configuration::GetWindowHeight() const{
  return window_height;
}

int Configuration::GetWindowWidth() const{
  return window_width;
}

int* Configuration::GetWindowHeightAddress(){
  return &window_height;
}
int* Configuration::GetWindowWidthAddress(){
  return &window_width;
}

std::vector<StellarBody> Configuration::GetStellarList() const{
  return stellar_list;
}