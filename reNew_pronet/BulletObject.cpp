#include "BulletObject.h"

BulletObject::BulletObject(const char* _src
    , const pronet::poolObject_shared_ptr<Shader, 6>& _shader
    , const pronet::poolObject_shared_ptr<Object, 6>& _object
    , const pronet::poolObject_shared_ptr<Texture, 6>& _texture)
	: ptr_(nullptr)
	, script_(_src ? py_obj::createPythonFunctionObject(_src) : 0)
    , shader_(_shader)
    , object_(_object)
    , texture_(_texture)
{
    if (script_) {
        try {
            py::object setup = py_obj::getFunctionObject(script_, "setup");
            py::call<void>(setup.ptr());
            py_obj::createBulletObjectByPython();
            generate_ = py_obj::getFunctionObject(script_, "generate");
            update_ = py_obj::getFunctionObject(script_, "update");
        }
        catch (const py::error_already_set&) {
            PyErr_Print();
        }
        ptr_ = data_base::getBulletManager(script_);
    }
}

void BulletObject::init(const char* _src
    , const pronet::poolObject_shared_ptr<Shader, 6>& _shader, const pronet::poolObject_shared_ptr<Object, 6>& _object
    , const pronet::poolObject_shared_ptr<Texture, 6>& _texture)
{
	script_ = py_obj::createPythonFunctionObject(_src);
    try {
        py::object setup = py_obj::getFunctionObject(script_, "setup");
        setup();
        py_obj::createBulletObjectByPython();
        shader_ = _shader;
        object_ = _object;
        texture_ = _texture;
        generate_ = py_obj::getFunctionObject(script_, "generate");
        update_ = py_obj::getFunctionObject(script_, "update");
    }
    catch (const py::error_already_set&) {
        PyErr_Print();
    }
	ptr_ = data_base::getBulletManager(script_);
}

void BulletObject::execute(float _pos[2], bool _status, data_base::BULLET_PARAM_DB* const _data_base)
{
    if (!_data_base)
        throw std::runtime_error("BulletDataBase is null : BulletObject::execute(float, bool, data_base::BULLET_PARAM_DB* const)");

    update(_pos, _status, _data_base);
    draw(_data_base);
    if (_status) {
        generate(_pos, _status, _data_base);
    }
}

void BulletObject::update(float _pos[2], bool _status, data_base::BULLET_PARAM_DB* const _data_base)
{
    size_t index(0);
    for (auto& a : _data_base->bullets) {
        index = 0;
        py_obj::setBulletObjectDataBinding(&a.data);
        for (auto& b : a.bullet) {
            try {
                py::call<void>(update_.ptr(), _pos[0], _pos[1], _status, _data_base->data, boost::ref(a.data), script_, index, boost::ref(b));
            }
            catch (const py::error_already_set&) {
                PyErr_Print();
            }
            index++;
        }
    }

    if (!_data_base->bullets.empty()) {
        const auto& front = _data_base->bullets.front();
        if (front.data.radiculus > _data_base->data.area) {
            std::cout << front.data.radiculus << ">" << _data_base->data.area << std::endl;
            _data_base->bullets.erase(_data_base->bullets.begin());
        }
    }
}

void BulletObject::generate(float _pos[2], bool _status, data_base::BULLET_PARAM_DB* const _data_base)
{
    clock_t now_clock = clock();
    if (static_cast<uint32_t>(now_clock - lastClock) > _data_base->data.interval) {
        //  弾幕のオブジェクトのデータを生成する
        _data_base->bullets.emplace_back();
        data_base::BULLET_OBJECT_MANAGER& str = _data_base->bullets.back();
        try {
            py::call<void>(generate_.ptr(), _pos[0], _pos[1], _status, _data_base->data, _pos[0], _pos[1], 0.f, 0.f, script_, boost::ref(str.data));
        }
        catch (const py::error_already_set&) {
            PyErr_Print();
        }
        str.bullet.resize(_data_base->data.count);

        //  生成したデータを各弾幕に反映させる
        float now_deg(str.data.begin_degree);
        const float deg_count(_data_base->data.rad / _data_base->data.count);
        for (auto& a : str.bullet) {
            a.deg = now_deg;
            a.positionX = str.data.positionX;
            a.positionY = str.data.positionY;
            now_deg += deg_count;
        }
        lastClock = now_clock;
    }
}

void BulletObject::draw(data_base::BULLET_PARAM_DB* const _data_base)
{
    shader_->use();
    texture_->exe(shader_->getTextureUnitID());
    for (auto& a : _data_base->bullets) {
        pronet::updateUniformBlock(GAME_BULLET_DATA_PARAMETER, &a);
        for (auto& b : a.bullet) {
            pronet::updateUniformBlock(GAME_BULLET_PARAMETER, &b);
            object_->draw();
        }
    }
}

void BulletObject::run(float _pos[2], bool _status)
{
    py_obj::bindPythonBulletFuncObject(script_);
    py_obj::setBulletParamDataBinding(&ptr_->data_base.data);
	execute(_pos, _status, &ptr_->data_base);
}

void BulletObject::run(glm::vec2 _pos, bool _status)
{
    py_obj::bindPythonBulletFuncObject(script_);
    py_obj::setBulletParamDataBinding(&ptr_->data_base.data);

    float buf[2] = { _pos.x, _pos.y };
    execute(buf, _status, &ptr_->data_base);
    draw(&ptr_->data_base);
}

void BulletObject::reset()
{
	ptr_ = nullptr;
	script_ = 0;
    shader_.reset();
    object_.reset();
    texture_.reset();
	generate_ = py::object();
	update_ = py::object();
}