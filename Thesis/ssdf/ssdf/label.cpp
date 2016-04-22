#include "stdafx.h"
#include "label.h"
#include "color.h"

BEGIN_NAMESPACE_2(Pups,Label)
Simple::Simple():m_color(Color::CYAN){
}

Simple::Simple( const Color& color ) :m_color( color ){
}

Color Simple::GetColor(
    float x,
    float y,
    float w ){
    return m_color;
}
END_NAMESPACE_2