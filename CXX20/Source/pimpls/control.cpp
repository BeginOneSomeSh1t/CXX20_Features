#include "control.h"
#include <iostream>
#include <format>


class control_pimpl
{
    _Control_data _Data;

    void _Draw()
    {
        std::cout << std::format("control\n "
                                 " text: {}\n "
                                 " size: {} {} \n"
                                 " visible: {}",
                                 _Data._Text, _Data._Width, _Data._Height, _Data._Visible);
    }
public:

    control_pimpl() = default;
    control_pimpl(control_properties const& _Properties)
        :
    _Data(_Properties._Data)
    {  }
    
    void set_text(std::string_view _Text)
    {
        _Data._Text = _Text.data();
    }
    void resize(int const _Width, int const _Height)
    {
       _Data._Width = _Width;
        _Data._Height = _Height;
    }
    
    void show()
    {
        _Data._Visible = true;
        _Draw();
    }
    
    void hide()
    {
        _Data._Visible = false;
        _Draw();
    }
};



control::control()
    :
_Pimpl(new control_pimpl(), [](control_pimpl* _P){delete _P;})
{
}

control::control(control_properties const& _Prop)
    :
_Pimpl(new control_pimpl(_Prop), [](control_pimpl* _P){delete _P;})
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
