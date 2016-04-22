#include "stdafx.h"
#include "label_function.h"
#include "vertex.h"
#include "math.h"
BEGIN_NAMESPACE_2(Pups,LabelFunction)
RandomBinary::RandomBinary():m_fg(nullptr),m_bg(nullptr){
}

RandomBinary::RandomBinary( Label::Base* fg, Label::Base* bg ):
m_fg(fg),m_bg(bg){
}

void RandomBinary::AssignLabels( Vertices& vertices ){
    for ( Vertex* v : vertices ){
        float rnd = Math::Random();
        if ( rnd > 0.5f ){
            v->SetId( 1 );
            v->SetLabel( m_fg );
        }
        else{
            v->SetId( 0 );
            v->SetLabel( m_bg );
        }
    }
}

Solid::Solid():m_label(nullptr){
}

Solid::Solid( Label::Base* label ):m_label(label){
}

void Solid::AssignLabels( Vertices& vertices ){
    for ( Vertex* v : vertices ){
        v->SetId( 0 );
        v->SetLabel( m_label );
    }
}
END_NAMESPACE_2
