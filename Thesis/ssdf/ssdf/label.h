#pragma once
#include "stdafx.h"
#include "color.h"

BEGIN_NAMESPACE_2(Pups,Label)
//general label
class Base{
public:
    Base(){};
    virtual ~Base(){}

    virtual Color GetColor(
        float x,
        float y,
        float w) = 0;
};

//single color label
class Simple : public Base{
public:
    Simple();
    Simple( const Color& color );
    virtual ~Simple(){}

    virtual Color GetColor(
        float x,
        float y,
        float w );
private:
    Color m_color;
};
END_NAMESPACE_2
