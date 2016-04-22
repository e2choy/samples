#pragma once
#include "stdafx.h"

BEGIN_NAMESPACE(Pups)
class Vertex;
typedef std::vector<Vertex*> Vertices;
END_NAMESPACE

BEGIN_NAMESPACE_2(Pups,Label)
class Base;
END_NAMESPACE_2

BEGIN_NAMESPACE_2(Pups,LabelFunction)
///////////////////////////////////////////////////////
class Base{
public:
    virtual ~Base(){}
    virtual void AssignLabels( Vertices& vertices ) = 0;
};
///////////////////////////////////////////////////////
class None : public Base{
public:
    virtual void AssignLabels(Vertices& vertices){}
};
///////////////////////////////////////////////////////
class RandomBinary : public Base{
public:
    RandomBinary();
    RandomBinary( Label::Base* fg, Label::Base* bg );
    virtual void AssignLabels( Vertices& vertices );
private:
    Label::Base* m_fg;
    Label::Base* m_bg;
};
///////////////////////////////////////////////////////
class Solid : public Base{
public:
    Solid();
    Solid( Label::Base* label );
    virtual void AssignLabels( Vertices& vertices );
private:
    Label::Base* m_label;
};

END_NAMESPACE_2