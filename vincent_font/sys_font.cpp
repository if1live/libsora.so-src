// Ŭnicode please
#include "sys_font.h"
#include <algorithm>
#include <climits>
#include "vincent.h"

namespace haruna {;
namespace gl {

	std::unique_ptr<SysFont> g_sysFont;

	bool SysFont_Init()
	{
		if(g_sysFont.get() == nullptr) {
			g_sysFont.reset(new SysFont());
			g_sysFont->Init();
			return true;
		}
		return false;
	}
	
	bool SysFont_Deinit()
	{
		if(g_sysFont.get() != nullptr) {
			g_sysFont->Deinit();
			g_sysFont.reset(nullptr);
			return true;
		}
		return false;
	}

	FontVertex::FontVertex()
	{
		std::fill(p.begin(), p.end(), 0.0f);
		std::fill(uv.begin(), uv.end(), 0.0f);
	}
	FontVertex::FontVertex(float x, float y, float s, float t)
	{
		p[0] = x;
		p[1] = y;
		uv[0] = s;
		uv[1] = t;
	}

	SysFont::SysFont()
		: tex_id_(0)
	{	
	}
	SysFont::~SysFont() 
	{
	}
	bool SysFont::Init()
	{
		if(tex_id_ != 0) {
			return false;
		}

		// 텍스쳐는 1/0만 처리하면됨. 작게 할당받자
		typedef unsigned char byte;

		int img_size = sizeof(byte) * kTextureWidth * kTextureHeight;
		byte *data = static_cast<byte*>(malloc(img_size));
		std::fill(data, data + sizeof(byte) * img_size, 0);

		// 이미지 생성.
		for (int i = 0 ; i < 128 ; i++) {
			unsigned char *character_font = vincent_data[i];
			int base_x;
			int base_y;
			GetCharacterCoord(i, &base_x, &base_y);

			for (int y = 0 ; y < kFontSize ; y++) {
				char character_scanline = character_font[y];
				for (int x = 0 ; x < kFontSize ; x++) {
					int raw_pixel = (1 << (kFontSize - 1 - x)) & character_scanline;
					bool pixel = raw_pixel > 0 ? true : false;
					int pixel_index = GetCharacterPixelIndex(base_x, base_y, x, y);

					if(pixel == true) {
						data[pixel_index] = UCHAR_MAX;
					}
				}
			}
		}
		//create texture
		//Nearest로 써야된다. 그래야 텍스쳐 번지는게 없다
		glGenTextures(1, &tex_id_);
		glBindTexture(GL_TEXTURE_2D, tex_id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, kTextureWidth, kTextureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
		free(data);

		return true;
	}
	bool SysFont::Deinit()
	{
		if(tex_id_ == 0) {
			return false;
		}
		glDeleteTextures(1, &tex_id_);
		tex_id_ = 0;
		return true;
	}

	void SysFont::GetCharacterCoord(unsigned char ch, int *x, int *y) const 
	{
		*y = ch / kColCount;
		*x = ch % kColCount;
	}
	int SysFont::GetCharacterPixelIndex(int base_x, int base_y, int x, int y) const 
	{
		int pixel_x = base_x * kFontSize + x;
		int pixel_y = base_y * kFontSize + y;
		int index = pixel_y * kTextureWidth + pixel_x;
		return index;
	}

	void SysFont::GetCharacterTextureQuad(unsigned char ch, float *left, float *right, float *top, float *bottom) const 
	{
		int x, y;
		GetCharacterCoord(ch, &x, &y);
		y = kRowCount - y - 1;
		*left = x * (1.0f / kColCount);
		*right = (x + 1) * (1.0f / kColCount);
		*bottom = 1 - y * (1.0f / kRowCount);
		*top = 1 - (y + 1) * (1.0f / kRowCount);
	}

	///////////////////////////////////
	Label::Vertex CreateVertex(float x, float y, float s, float t) 
	{
		Label::Vertex v(x, y, s, t);
		return v;
	}

	Label::Label(SysFont *font) 
		: font_(font) 
	{
		Init("");
	}
	Label::Label(SysFont *font, const std::string &msg) 
		: font_(font), text_(msg) 
	{
		Init(text_);
	}
	Label::~Label() 
	{
	}

	
	void Label::Init(const std::string &text) 
	{
		VertexList &vertex_list = vert_list_;
		IndexList &index_list = index_list_;

		for (size_t i = 0 ; i < text.size() ; i++) {
			unsigned char ch = text[i];
			float tex_left, tex_right, tex_top, tex_bottom;
			font_->GetCharacterTextureQuad(ch, &tex_left, &tex_right, &tex_top, &tex_bottom);

			float left, right, top, bottom;
			left = i * (float)SysFont::kFontSize;
			right = left + (float)SysFont::kFontSize;
			top = 0.0f;
			bottom = top - (float)SysFont::kFontSize;

			// 3 2
			// 0 1
			Vertex v0 = CreateVertex(left, bottom, tex_left, tex_bottom);
			Vertex v1 = CreateVertex(right, bottom, tex_right, tex_bottom);
			Vertex v2 = CreateVertex(right, top, tex_right, tex_top);
			Vertex v3 = CreateVertex(left, top, tex_left, tex_top);

			int base_index = vertex_list.size();

			vertex_list.push_back(v0);
			vertex_list.push_back(v1);
			vertex_list.push_back(v2);
			vertex_list.push_back(v3);

			index_list.push_back(base_index + 0);
			index_list.push_back(base_index + 1);
			index_list.push_back(base_index + 2);

			index_list.push_back(base_index + 0);
			index_list.push_back(base_index + 2);
			index_list.push_back(base_index + 3);
		}
	}

	const Label::Vertex *Label::vertex_data() const 
	{
		if(vert_list_.empty()) {
			return NULL;
		}
		return &vert_list_[0];
	}
	const void *Label::index_data() const 
	{
		if(index_list_.empty()) {
			return NULL;
		}
		return &index_list_[0];
	}

	void Label::set_text(const std::string &text) 
	{
		if(text_ == text) {
			return;
		}
		text_ = text;
		vert_list_.clear();
		index_list_.clear();
		Init(text);
	}
}	// namespace gl
}	// namespace haruna
