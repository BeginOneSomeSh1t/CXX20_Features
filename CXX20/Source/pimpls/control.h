#pragma once
#include <memory>
#include <string_view>

class control_pimpl;

struct _Control_data
{
    std::string _Text;
    int _Width;
    int _Height;
    bool _Visible = true;
};

class control_properties
{
    _Control_data _Data;
    friend class control_pimpl;
public:
    control_properties() = default;
    
    control_properties& text(std::string_view _Text){ _Data._Text = _Text; return *this; }
    control_properties& width(int const _Wid){ _Data._Width = _Wid; return *this; }
    control_properties& height(int const _Hght){ _Data._Width = _Hght; return *this; }
    control_properties& visible(bool const _Visible){ _Data._Visible = _Visible; return *this; }
};

class control
{
    std::unique_ptr<control_pimpl, void(*)(control_pimpl*)> _Pimpl;
public:

    control();
    control(class control_properties const& _Prop);
    control(control&&) noexcept;
    control& operator=(control&&) noexcept;
    
    control(control const& _Other);
    control& operator=(control const& _Other);

    void set_text(std::string_view _Text);
    void resize(int const _Width, int const _Height);
    void show();
    void hide();
};