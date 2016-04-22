#pragma once
#include "stdafx.h"

BEGIN_NAMESPACE(Pups)
class Mesh;
class Color;

END_NAMESPACE

BEGIN_NAMESPACE_2( Pups, Graphics )
void DrawMesh( const Mesh& mesh, const Color& color, Mat& img );
void FillConvexPoly( const Points& pts, int val, Mat& img );
void DebugImage( const string& str, int index, const Mat& img );
END_NAMESPACE_2