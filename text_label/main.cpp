// Ŭnicode please 
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include "text_label.h"

const int kWidth = 640;
const int kHeight = 480;

using namespace haruna;

int main()
{
	GLFWwindow* window = nullptr;

	if(!glfwInit()) {
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(kWidth, kHeight, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	OpenGLFontDevice::Init(kWidth, kHeight);
	TexFont::GenerateDefaultFonts();
	std::unique_ptr<TexFont> normal_font = TexFont::GenerateNormalFont(1.0f);

	TextLabel label;
	std::string msg1("This is Some Message!");
	TextLabelConfig cfg;
	label.BuildText(msg1, g_DefaultNormalFont.get(), cfg);

	int running = GL_TRUE;
	while (!glfwWindowShouldClose(window) && running) {
		g_fontDevice->FrameStart();

		glViewport(0, 0, kWidth, kHeight);
		// OpenGL rendering goes here...
		glClearColor(0, 0, 0, 0);
		glClear( GL_COLOR_BUFFER_BIT );

		// 왼쪽아래를 기준점으로 잡고 렌더링 하는거
		g_fontDevice->Set2D(kCoordLeftBottom);
		{
			TextLabel label;
			std::string msg1("This is Some Message!");
			TextLabelConfig cfg;
			label.BuildText(msg1, g_DefaultNormalFont.get(), cfg);
			g_fontDevice->DrawText(&label, 0, 0, COLOR32_WHITE, COLOR32_WHITE);
		}
		{
			TextLabel label;
			std::string msg1("Position Move(100, 200)");
			TextLabelConfig cfg;
			label.BuildText(msg1, normal_font.get(), cfg);
			g_fontDevice->DrawText(&label, 100, 200, COLOR32_WHITE, COLOR32_WHITE);
		}
		{
			TextLabel label;
			std::vector<std::string> lines;
			lines.push_back(std::string("1.Multi"));
			lines.push_back(std::string("2.Line"));

			TextLabelConfig cfg;
			label.BuildText(lines, g_DefaultFixed1Font.get(), cfg);
			g_fontDevice->DrawText(&label, 100, 100, COLOR32_GREEN, COLOR32_WHITE);
		}
		{
			TextLabel label;
			std::string msg1("Color");
			color32 line_colors[] = { COLOR32_BLUE, };
			color32 bg_colors[] = { COLOR32_RED, };
			TextLabelConfig cfg;
			cfg.line_colors = line_colors;
			cfg.line_bg_colors = bg_colors;
			cfg.bg_width = 100;
			label.BuildText(msg1, g_DefaultSmallFont.get(), cfg);
			g_fontDevice->DrawText(&label, 200, 250, 0, 0);
		}

		//원점이 오른쪽위
		g_fontDevice->Set2D(kCoordLeftTop);
		{
			TextLabel label;
			std::string msg1("Left-Top");
			TextLabelConfig cfg;
			cfg.coord = kCoordLeftTop;
			label.BuildText(msg1, g_DefaultNormalFont.get(), cfg);
			g_fontDevice->DrawText(&label, 0, 0, COLOR32_WHITE, COLOR32_WHITE);
		}
		{
			TextLabel label;
			std::string msg1("Left-Top Position Move(100, 50)");
			TextLabelConfig cfg;
			cfg.coord = kCoordLeftTop;
			label.BuildText(msg1, normal_font.get(), cfg);
			g_fontDevice->DrawText(&label, 100, 50, COLOR32_WHITE, COLOR32_RED);
		}
		{
			TextLabel label;
			std::array<std::string, 2> lines;
			lines[0] = std::string("1.Multi---");
			lines[1] = std::string("2.Line---");

			TextLabelConfig cfg;
			cfg.coord = kCoordLeftTop;
			color32 line_colors[] = { COLOR32_BLUE, COLOR32_RED };
			color32 bg_colors[] = { COLOR32_RED, COLOR32_GREEN };
			cfg.line_colors = line_colors;
			cfg.line_bg_colors = bg_colors;
			cfg.bg_width = 100;
			cfg.sep = 10;
			label.BuildText(lines, g_DefaultFixed1Font.get(), cfg);
			g_fontDevice->DrawText(&label, 100, 100, 0, 0);
		}


		// Swap front and back rendering buffers
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		// Check if ESC key was pressed or window was closed
		running = !glfwGetKey(window, GLFW_KEY_ESCAPE);
	}

	OpenGLFontDevice::Deinit();
	glfwTerminate();

	return 0;
}