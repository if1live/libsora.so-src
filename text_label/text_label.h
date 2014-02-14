// Ŭnicode please
#pragma once
//  ---------------------------------------------------------------------------
//
//  @file       TwFonts.h
//  @brief      Bitmaps fonts
//  @author     Philippe Decaudin
//  @license    This file is part of the AntTweakBar library.
//              For conditions of distribution and use, see License.txt
//
//  note:       Private header
//
//  ---------------------------------------------------------------------------

/*
A source bitmap includes 224 characters starting from ascii char 32 (i.e. space) 
to ascii char 255 (extended ASCII Latin1/CP1252):

!"#$%&'()*+,-./0123456789:;<=>?
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_
`abcdefghijklmnopqrstuvwxyz{|}~
걗깂뀊뇠뎷땶뜋룓몤뱮븭뿕솞썫씆?
젩∀ㄵ├ŁかЛ?갚껙뉘떱많본섹씩
읒쩠컵판훔墾勁卦今錄桐虜妹膊覆泗
褻栒鴨鉛僥幽日全猝膣逮擢票亨涍?

First pixel column of a source bitmap is a delimiter with color=zero at the end of each line of characters.
Last pixel row of a line of characters is a delimiter with color=zero at the last pixel of each character.
*/

#include <vector>
#include <array>
#include <memory>
#include <type_traits>

#if (defined(_WIN32) || defined(_W64))
#include "windows.h"
#endif
#include <GL/gl.h>

namespace haruna {;

typedef unsigned int color32;

const color32 COLOR32_BLACK     = 0xff000000;   // Black 
const color32 COLOR32_WHITE     = 0xffffffff;   // White 
const color32 COLOR32_ZERO      = 0x00000000;   // Zero 
const color32 COLOR32_RED       = 0xffff0000;   // Red 
const color32 COLOR32_GREEN     = 0xff00ff00;   // Green 
const color32 COLOR32_BLUE      = 0xff0000ff;   // Blue 

typedef enum {
	kCoordLeftTop,
	kCoordLeftBottom,
} CoordType;

class Vec2 { 
public:
	float x, y; 
	Vec2() {} 
	Vec2(float _X, float _Y) : x(_X), y(_Y) {} 
	Vec2(int _X, int _Y) : x(float(_X)), y(float(_Y)) {} 
};

class TexFont {
public:
	unsigned char * m_TexBytes;
	int             m_TexWidth;     // power of 2
	int             m_TexHeight;    // power of 2
	float           m_CharU0[256];
	float           m_CharV0[256];
	float           m_CharU1[256];
	float           m_CharV1[256];
	int             m_CharWidth[256];
	int             m_CharHeight;
	int             m_NbCharRead;

	TexFont();
	~TexFont();

public:
	static std::unique_ptr<TexFont> GenerateSmallFont(float scaling=1.0f);
	static std::unique_ptr<TexFont> GenerateNormalFont(float scaling=1.0f);
	static std::unique_ptr<TexFont> GenerateLargeFont(float scaling=1.0f);
	static std::unique_ptr<TexFont> GenerateFixed1Font(float scaling=1.0f);

	static std::unique_ptr<TexFont> GenerateFont(const unsigned char *_Bitmap, int _BmWidth, int _BmHeight, float _Scaling=1.0f);

public:
	static void GenerateDefaultFonts(float _Scaling=1.0f);
};
extern std::unique_ptr<TexFont> g_DefaultSmallFont;
extern std::unique_ptr<TexFont> g_DefaultNormalFont;
extern std::unique_ptr<TexFont> g_DefaultLargeFont;
extern std::unique_ptr<TexFont> g_DefaultFixed1Font;

class TextLabelConfig {
public:
	TextLabelConfig()
		: coord(kCoordLeftBottom),
		line_colors(nullptr),
		line_bg_colors(nullptr),
		sep(1),
		bg_width(0) {}
	CoordType coord;
	color32 *line_colors;
	color32 *line_bg_colors;
	int sep;
	int bg_width;
};

class TextLabel {
public:
	TextLabel() : font(nullptr) {}

	std::vector<Vec2> m_TextVerts;
	std::vector<Vec2> m_TextUVs;
	std::vector<Vec2> m_BgVerts;
	std::vector<color32> m_Colors;
	std::vector<color32> m_BgColors;

	const TexFont *font;

public:
	//void BuildText(const std::string &_Text, color32 *_LineColors, color32 *_LineBgColors, const TexFont &_Font, int _Sep, int _BgWidth);

	template<typename StringContainer>
	void BuildText(const StringContainer &_TextLines, const TexFont *_Font, const TextLabelConfig &cfg);
	template<>
	void BuildText(const std::string &_Text, const TexFont *_Font, const TextLabelConfig &cfg);
};


class OpenGLFontDevice {
public:
	static bool Init(int width, int height);
	static bool Deinit();

public:
	void FrameStart();

	~OpenGLFontDevice();

	void DrawText(TextLabel *_TextObj, int _X, int _Y, color32 _Color, color32 _BgColor);
	void Set2D(CoordType coord);

	GLuint BindFont(const TexFont &_Font);
	void UnbindFont(GLuint _FontTexID);

private:
	OpenGLFontDevice(int width, int height);

	int width_;
	int height_;

	const TexFont *last_font_;
	GLuint last_tex_id_;
};
extern std::unique_ptr<OpenGLFontDevice> g_fontDevice;


// template 
template<typename StringContainer>
void TextLabel::BuildText(const StringContainer &_TextLines, const TexFont *_Font, const TextLabelConfig &cfg)
{
	static_assert(std::is_same<std::string, StringContainer::value_type>::value == 1, 
		"allow only value_type=std::string");

	this->font = _Font;

	this->m_TextVerts.resize(0);
	this->m_TextUVs.resize(0);
	this->m_BgVerts.resize(0);
	this->m_Colors.resize(0);
	this->m_BgColors.resize(0);

	int x, x1, y, y1, i, Len;
	unsigned char ch;
	const unsigned char *Text;
	color32 LineColor = COLOR32_RED;

	auto it = _TextLines.begin();
	auto endit = _TextLines.end();
	int Line = 0;
	for( ; it != endit ; ++it, ++Line) {
		x = 0;
		y = Line * (_Font->m_CharHeight+cfg.sep);
		y1 = y+_Font->m_CharHeight;
		Len = (int)_TextLines[Line].length();
		Text = (const unsigned char *)(_TextLines[Line].c_str());
		if( cfg.line_colors != nullptr) {
			LineColor = (cfg.line_colors[Line]&0xff00ff00) | GLubyte(cfg.line_colors[Line]>>16) | (GLubyte(cfg.line_colors[Line])<<16);
		}

		for( i=0; i<Len; ++i ) {
			ch = Text[i];
			x1 = x + _Font->m_CharWidth[ch];

			m_TextVerts.push_back(Vec2(x , y ));
			m_TextVerts.push_back(Vec2(x1, y ));
			m_TextVerts.push_back(Vec2(x , y1));
			m_TextVerts.push_back(Vec2(x1, y ));
			m_TextVerts.push_back(Vec2(x1, y1));
			m_TextVerts.push_back(Vec2(x , y1));

			
			if(cfg.coord == kCoordLeftTop) {
				m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV0[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV0[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV1[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV0[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV1[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV1[ch]));
			} else {
				m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV1[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV1[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV0[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV1[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU1[ch], _Font->m_CharV0[ch]));
				m_TextUVs.push_back(Vec2(_Font->m_CharU0[ch], _Font->m_CharV0[ch]));
			}

			if( cfg.line_colors != nullptr )	{
				m_Colors.push_back(LineColor);
				m_Colors.push_back(LineColor);
				m_Colors.push_back(LineColor);
				m_Colors.push_back(LineColor);
				m_Colors.push_back(LineColor);
				m_Colors.push_back(LineColor);
			}

			x = x1;
		}
		if( cfg.bg_width > 0 ) {
			m_BgVerts.push_back(Vec2(-1        , y ));
			m_BgVerts.push_back(Vec2(cfg.bg_width+1, y ));
			m_BgVerts.push_back(Vec2(-1        , y1));
			m_BgVerts.push_back(Vec2(cfg.bg_width+1, y ));
			m_BgVerts.push_back(Vec2(cfg.bg_width+1, y1));
			m_BgVerts.push_back(Vec2(-1        , y1));

			if( cfg.line_bg_colors != nullptr) {
				color32 LineBgColor = (cfg.line_bg_colors[Line]&0xff00ff00) | GLubyte(cfg.line_bg_colors[Line]>>16) | (GLubyte(cfg.line_bg_colors[Line])<<16);
				m_BgColors.push_back(LineBgColor);
				m_BgColors.push_back(LineBgColor);
				m_BgColors.push_back(LineBgColor);
				m_BgColors.push_back(LineBgColor);
				m_BgColors.push_back(LineBgColor);
				m_BgColors.push_back(LineBgColor);
			}
		}
	}
}

template<>
void TextLabel::BuildText(const std::string &_Text, const TexFont *_Font, const TextLabelConfig &cfg)
{
	std::array<std::string, 1> textLine = { _Text };
	BuildText(textLine, _Font, cfg);
}
}	// namespace haruna