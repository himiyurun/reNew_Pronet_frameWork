#include "loadPronetMap2.h"

pronet::loadPronetMap2::loadPronetMap2()
    : file_name(nullptr)
{
}

bool pronet::loadPronetMap2::get_mapInfo(const char* name, poolArray_unique_ptr<Structure2vCreateInfo> info[5])
{
    if (!read_file_init(name)) {
        return false;
    }
    if (!type_is()) {
        throw std::logic_error("pronetmapfile header is wrong!");
        return false;
    }
    get_script(info);
    if (!file_close()) {
        throw std::runtime_error("pronetmapfile can't close!");
        return false;
    }
    std::cout << "read_end" << std::endl;
    return true;
}

void pronet::loadPronetMap2::return_create_info(PoolArray<Structure2vCreateInfo> info[5])
{
    for (int i = 0; i < CHANCK_DIRECTION_SIZE; i++) {
        pool.back(std::move(&info[i]));
        if (info[i].data != nullptr) {
            std::cerr << "object object object object object" << std::endl;
        }
    }
}

bool pronet::loadPronetMap2::read_file_init(const char* name)
{
    ifs.open(name, std::ios::in);
    if (!ifs.is_open()) {
        std::string err(name);
        err += " is not found!";
        throw std::logic_error(err);
        return false;
    }
    file_name = name;
    return true;
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

void pronet::loadPronetMap2::get_struct(poolArray_unique_ptr<Structure2vCreateInfo>& info)
{
    size_t str_size(0);
    
    clear_string();
    line_getting_by_text();
    iss >> script;
    script_func("size", [this, &str_size]() {
        iss >> str_size;
        });
    if (str_size == 0) return;
    clear_string();

    info.realloc(str_size, &pool);
    for (size_t i = 0; i < str_size; i++) {
        line_getting_by_text();
        iss >> info[i].shader_index          //  シェーダーのインデックス
            >> info[i].buffer_object_index   //  バッファオブジェクトのインデックス
            >> info[i].texture_index         //  テクスチャのインデックス
            >> info[i].param.location[0]     //  ワールド座標系のx座標
            >> info[i].param.location[1]     //  ワールド座標系のy座標
            >> info[i].param.col_pos[0]      //  当たり判定をとる左上のオブジェクト座標系のx座標
            >> info[i].param.col_pos[1]      //  当たり判定をとる左上のオブジェクト座標系のy座標
            >> info[i].param.col_size[0]     //  当たり判定をとるx方向の大きさ
            >> info[i].param.col_size[1]     //  当たり判定をとるy方向の大きさ
            >> info[i].param.rotate;         //  オブジェクトの角度
        clear_string();
    }
}

void pronet::loadPronetMap2::structure_by_script(const char* text, poolArray_unique_ptr<Structure2vCreateInfo>& info)
{
    script_func(text, [this, &info]() {
        iss >> script;
        if (strcmp("{", script.c_str()) == 0) {
            get_struct(info);
            line_getting_by_text();
            iss >> script;
        }
        });
}

void pronet::loadPronetMap2::line_getting_by_text()
{
    std::getline(ifs, line);
    iss.str(line);
}

void pronet::loadPronetMap2::get_script(poolArray_unique_ptr<Structure2vCreateInfo> info[5])
{
    while (!ifs.eof()) {
        line_getting_by_text();
        iss >> script;
        switch (script[0]) {
        case 'B':
            break;
        case 'M':
            break;
        case 'b':
            structure_by_script("b_up", info[CHANCK_BOUNDARY_UP]);
            structure_by_script("b_down", info[CHANCK_BOUNDARY_DOWN]);
            structure_by_script("b_right", info[CHANCK_BOUNDARY_RIGHT]);
            structure_by_script("b_left", info[CHANCK_BOUNDARY_LEFT]);
            break;
        case 's':
            structure_by_script("str", info[CHANCK_NATIVE]);
            break;
        case '/':
            script_func("//", [](){});
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
        return false;
    }
    ifs.clear();
    clear_string();
    return true;
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

pronet::ObjectPool_Array<Structure2vCreateInfo> pronet::loadPronetMap2::pool(32);