#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct StellarBody {
    std::string name;
    std::string type;
    std::string path_to_obj;
    std::string path_to_mtl;

    float aphelion;
    float periphelion;
    float rotate_speed;
    float axial_tilt;
    float rotational_period;
    float orbital_speed;

    StellarBody(){
        name = "";
        type = "";
        path_to_obj = "";
        path_to_mtl = "";

        aphelion = 0.0;
        periphelion = 0.0;
        rotate_speed = 0.0;
        axial_tilt = 0.0;
        rotational_period = 0.0;
        orbital_speed = 0.0;
    }
};

class Configuration
{
public:
    Configuration();
    ~Configuration();

    std::vector<std::string> Split(std::string str);
    bool LoadConfigFile(std::string f_path);

    int GetStellarBodyCount() const;
    std::string GetVShaderFilePath() const;
    std::string GetFShaderFilePath() const;
    std::string GetWindowName() const;
    int GetWindowHeight() const;
    int GetWindowWidth() const;
    int* GetWindowHeightAddress();
    int* GetWindowWidthAddress();
    std::vector<StellarBody> GetStellarList() const;

private:
    int stellar_body_count;

    std::string v_shader_file_path;
    std::string f_shader_file_path;

    std::string window_name;
    int window_height;
    int window_width;

    std::vector<StellarBody> stellar_list;
};
#endif //CONFIG_H