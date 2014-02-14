// Ŭnicode please
#include <cstdlib>
#include <GLFW/glfw3.h>
#include "sys_font.h"

const int kWidth = 300;
const int kHeight = 300;

using namespace haruna::gl;

void RenderLabel(const haruna::gl::Label &label, float x, float y)
{
	glPushMatrix();
	{
		glTranslatef(x, y, 0);
		glVertexPointer(2, GL_FLOAT, sizeof(FontVertex), &label.vertex_list()[0].p);
		glTexCoordPointer(2, GL_FLOAT, sizeof(FontVertex), &label.vertex_list()[0].uv);
		glDrawElements(GL_TRIANGLES, label.index_count(), GL_UNSIGNED_SHORT, label.index_data());
	}
	glPopMatrix();
}

int main()
{
	GLFWwindow* window = nullptr;

	/* Initialize the library */
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

	//initialize
	haruna::gl::SysFont_Init();

	//set default gl env
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	int running = GL_TRUE;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && running) {
		glViewport(0, 0, kWidth, kHeight);
		// OpenGL rendering goes here...
		glClearColor(0, 0, 0, 0);
		glClear( GL_COLOR_BUFFER_BIT );

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, kWidth, 0, kHeight, 0, 100);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		

		//bind font texture + set env
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_sysFont->tex_id());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//라벨의 왼쪽아래가 0,0이다
		RenderLabel(Label(g_sysFont.get(), "This is BaseLine"), 0, SysFont::kFontSize);
		RenderLabel(Label(g_sysFont.get(), "SomeText"), 100, 50);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		RenderLabel(Label(g_sysFont.get(), "Blue Text? 1 2 3 4 @ # %"), 0, 100);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		// Swap front and back rendering buffers
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		// Check if ESC key was pressed or window was closed
		running = !glfwGetKey(window, GLFW_KEY_ESCAPE);
	}


	//cleanup
	haruna::gl::SysFont_Deinit();
	glfwTerminate();
	return 0;
}
