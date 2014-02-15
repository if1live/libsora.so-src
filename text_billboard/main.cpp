// Ŭnicode please
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "sys_font.h"
#include "primitive_mesh.h"

const int kWidth = 300;
const int kHeight = 300;

using namespace glm;
using namespace haruna;
using namespace haruna::gl;

glm::mat4 g_proj_mat;
glm::mat4 g_view_mat;
glm::mat4 g_model_mat;

GLenum DrawModeToGLMode(haruna::DrawType draw_mode)
{
	switch(draw_mode) {
	case kDrawPoints:
		return GL_POINTS;
	case kDrawLines:
		return GL_LINES;
	case kDrawLineStrip:
		return GL_LINE_STRIP;
	case kDrawLineLoop:
		return GL_LINE_LOOP;
	case kDrawTriangles:
		return GL_TRIANGLES;
	case kDrawTriangleStrip:
		return GL_TRIANGLE_STRIP;
	case kDrawTriangleFan:
		return GL_TRIANGLE_FAN;
	default:
		assert(!"do not reach");
		return GL_TRIANGLES;
	}
}

void RenderMesh(const std::vector<DrawCmdData<Vertex_1P1N1UV>> &draw_cmd_list)
{
    auto it = draw_cmd_list.begin();
    auto endit = draw_cmd_list.end();
    for( ; it != endit ; ++it) {
        const DrawCmdData<Vertex_1P1N1UV> &draw_cmd =  *it;
        GLenum mode = DrawModeToGLMode(draw_cmd.draw_mode);
        int stride = sizeof(Vertex_1P1N1UV);
        glVertexPointer(3, GL_FLOAT, stride, &draw_cmd.vertex_list[0].p);
        glTexCoordPointer(2, GL_FLOAT, stride, &draw_cmd.vertex_list[0].uv);
        glNormalPointer(GL_FLOAT, stride, &draw_cmd.vertex_list[0].n);
        glDrawElements(mode, draw_cmd.index_list.size(), GL_UNSIGNED_SHORT, &draw_cmd.index_list[0]);
    }
}

void RenderBillboardLabel(const haruna::gl::Label &label, float x, float y, float z)
{	
	mat4 mvp = g_proj_mat * g_view_mat * g_model_mat;
	
	//billboard 같은 느낌으로 글자 쓰기
	//기울어지는거 없이 항상 글자가 뜨도록 적절히 만들기
	vec4 cliping_pos = mvp * vec4(x, y, z, 1);   // -1~1에 적절히 위치한 좌표
	cliping_pos /= cliping_pos.w;
	cliping_pos.z = -cliping_pos.z; //보정된 좌표계는 z방향 다르다

	// -1~+1로 보정된 좌표를 윈도우좌표로 변환
	vec3 win_coord(
		(cliping_pos.x+1) * kWidth/2.0f,
		(cliping_pos.y+1) * kHeight/2.0f,
		cliping_pos.z
		);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, kWidth, 0, kHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(win_coord.x, win_coord.y, win_coord.z);
	
	glVertexPointer(2, GL_FLOAT, sizeof(FontVertex), &label.vertex_list()[0].p);
	glTexCoordPointer(2, GL_FLOAT, sizeof(FontVertex), &label.vertex_list()[0].uv);
	glDrawElements(GL_TRIANGLES, label.index_count(), GL_UNSIGNED_SHORT, label.index_data());

	{
		// restore 3d matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(glm::value_ptr(g_proj_mat));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glm::mat4 modelview = g_view_mat * g_model_mat;
		glLoadMatrixf(glm::value_ptr(modelview));
	}
}

int main()
{
	GLFWwindow *window = nullptr;
	if(!glfwInit()) {
		exit(EXIT_FAILURE);
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
	glEnableClientState(GL_NORMAL_ARRAY);

	//create cube
	SolidCubeFactory factory(2, 2, 2);
	auto solid_cube_mesh = factory.CreateNormalMesh();

	float rot = 0;
	int running = GL_TRUE;
	while (!glfwWindowShouldClose(window) && running) {
		glViewport(0, 0, kWidth, kHeight);
		// OpenGL rendering goes here...
		glClearColor(0, 0, 0, 0);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glShadeModel(GL_SMOOTH);

		//render 3d
		{
			g_proj_mat = glm::perspective(glm::radians(60.0f), (float)(kWidth / kHeight), 0.1f, 100.0f);
			g_view_mat = glm::lookAt(glm::vec3(0, 2, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(g_proj_mat));

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(g_view_mat));

			g_model_mat = glm::rotate(glm::mat4(), rot, glm::vec3(0, 1, 0));
			glMultMatrixf(glm::value_ptr(g_model_mat));

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };		
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
			
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			
			RenderMesh(solid_cube_mesh);
		}
		
		//render 2d
		{
			glDisable(GL_LIGHTING);

			//bind font texture + set env
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, g_sysFont->tex_id());

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			RenderBillboardLabel(Label(g_sysFont.get(), "1/1/1"), 1, 1, 1);
			RenderBillboardLabel(Label(g_sysFont.get(), "1/1/-1"), 1, 1, -1);
			RenderBillboardLabel(Label(g_sysFont.get(), "1/-1/1"), 1, -1, 1);
			RenderBillboardLabel(Label(g_sysFont.get(), "1/-1/-1"), 1, -1, -1);

			RenderBillboardLabel(Label(g_sysFont.get(), "-1/1/1"), -1, 1, 1);
			RenderBillboardLabel(Label(g_sysFont.get(), "-1/1/-1"), -1, 1, -1);
			RenderBillboardLabel(Label(g_sysFont.get(), "-1/-1/1"), -1, -1, 1);
			RenderBillboardLabel(Label(g_sysFont.get(), "-1/-1/-1"), -1, -1, -1);
			
			RenderBillboardLabel(Label(g_sysFont.get(), "--------- 0/0/0"), 0, 0, 0);
		}

		assert(glGetError() == GL_NO_ERROR);
		// Swap front and back rendering buffers
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		// Check if ESC key was pressed or window was closed
		running = !glfwGetKey(window, GLFW_KEY_ESCAPE);

		rot += glm::radians(0.03f);
		
	}

	//cleanup
	haruna::gl::SysFont_Deinit();
	glfwTerminate();
	return 0;
}
