#pragma once
#include <memory>
#include <string_view>

class control_pimpl;

class control
{
    std::unique_ptr<control_pimpl, void(*)(control_pimpl*)> _Pimpl;
    control();
    control(control&&) noexcept;
    control& operator=(control&&) noexcept;
    
    control(control const& _Other);
    control& operator=(control const& _Other);

public:
    void set_text(std::string_view _Text);
    void resize(int const _Width, int const _Height);
    void show();
    void hide();
};