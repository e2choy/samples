#pragma once

#include "stdafx.h"

namespace Comp5900
{
	class App : public ofBaseApp
	{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
	private:
		void		ShowStage1();
		void		ShowStage2();
		void		ShowStage3();

		void		ShowStage4Before();
		void		ShowStage4After();

		void		ShowStage5a();
		void		ShowStage5b();
		void		ShowStage5c();

		void		ShowStage6a();
		void		ShowStage6b();

		void		ShowStage7();

		void		LocalLaplacian( const ofImage& image, float sigma, float alpha, const std::string& filename );
		void		LocalLaplacianColor( const ofImage& _image, float sigma, float alpha, const std::string& filename );
		void		LocalLaplacianHSV( const ofImage& _image, float sigma, float alpha, const std::string& filename );

		int			m_index;
		ofImage		m_image;
		ofImage		m_churchImage;
		ofImage		m_kittenImage;
		ofImage		m_flowerImage;
		ofImage		m_current;
	};
};
