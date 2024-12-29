#include "loadPronetMap2.h"

bool pronet::loadPronetMap2::read_file_init(const char* name)
{
    ifs.open(name, std::ios::in);
    if (!ifs.is_open()) {
        std::string err(name);
        err += " is not found!";
        throw std::logic_error(err);
    }
    file_name = name;
}

bool pronet::loadPronetMap2::type_is()
{
    std::getline(ifs, line);
    if (std::strcmp(line.c_str(), "#PRONET CHANCK MAPPING 2D") == 0) {
        clear_string();
        return true;
    }
    clear_string();
    return false;
}

void pronet::loadPronetMap2::get_script(std::unique_ptr<Structure2vCreateInfo>&info)
{
    if (!type_is()) {
        throw std::logic_error("file type is not correct!!");
    }

    while (std::getline(ifs, line)) {
        iss.str(line);
        iss >> script;
        switch (script[0]) {
        case 'B':
            script_func("Block_size", [this](){
                int height, width;
                iss >> height >> width;
                std::cout << "Block size : " << height << " " << width << std::endl;
                });
            break;
        case 'M':
            break;
        case 'b':
            break;
        case 's':
            break;
        default:
            break;
        }
        clear_string();
    }
}

bool pronet::loadPronetMap2::file_close()
{
    ifs.close();
    if (ifs.fail()) {
        std::string err(file_name);
        err += " can't close!";
        throw std::runtime_error(err);
    }
    clear_string();
}

void pronet::loadPronetMap2::script_func(const char* text, std::function<void()> func)
{
    if (std::strcmp(script.c_str(), text) == 0) {
        func();
    }
}

void pronet::loadPronetMap2::clear_string()
{
    line.clear();
    script.clear();
    iss.clear();
}
