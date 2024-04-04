#include "control.h"
#include <iostream>
#include <format>

class control_pimpl
{
    std::string _Text;
    int _Width;
    int _Height;
    bool _Visible = true;

    void _Draw()
    {
        std::cout << std::format("control\n "
                                 " text: {}\n "
                                 " size: {} {} \n"
                                 " visible: {}",
                                 _Text, _Width, _Height, _Visible);
    }
public:
    void set_text(std::string_view _Text)
    {
        this->_Text = _Text.data();
    }
    void resize(int const _Width, int const _Height)
    {
        this->_Width = _Width;
        this->_Height = _Height;
    }
    
    void show()
    {
        _Visible = true;
        _Draw();
    }
    
    void hide()
    {
        _Visible = false;
        _Draw();
    }
};



control::control()
    :
_Pimpl(new control_pimpl(), [](control_pimpl* _P){delete _P;})
{
}

control::control(control&&) noexcept = default;

control& control::operator=(control&&) noexcept = default;

control::control(control const& _Other)
    :
_Pimpl(new control_pimpl(*_Other._Pimpl),
    [](control_pimpl* _P){delete _P;})
{
}

control& control::operator=(control const& _Other)
{
    if(this != &_Other)
    {
        _Pimpl = std::unique_ptr<control_pimpl, void(*)(control_pimpl*)>(
            new control_pimpl(*_Other._Pimpl),
            [](control_pimpl* _P){delete _P;}
        );
    }
    return *this;
}

void control::set_text(std::string_view _Text)
{
    _Pimpl->set_text(_Text);
}

void control::resize(int const _Width, int const _Height)
{
    _Pimpl->resize(_Width, _Height);
}

void control::show()
{
    _Pimpl->show();
}

void control::hide()
{
    _Pimpl->hide();
}
