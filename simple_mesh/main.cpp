// Ŭnicode please
#ifdef _MSC_VER
	#include <Windows.h>
#endif
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cassert>

#include "teapot.h"
#include "primitive_mesh.h"

using namespace haruna;

const int kWidth = 300;
const int kHeight = 300;

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

void Draw(std::vector<DrawCmdData<Vertex_1P1N1UV>> &draw_cmd_list)
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

void Draw(std::vector<DrawCmdData<Vertex_1P>> &draw_cmd_list)
{
	auto it = draw_cmd_list.begin();
	auto endit = draw_cmd_list.end();
	for( ; it != endit ; ++it) {
		const DrawCmdData<Vertex_1P> &draw_cmd =  *it;
		GLenum mode = DrawModeToGLMode(draw_cmd.draw_mode);
		int stride = sizeof(Vertex_1P);
		glVertexPointer(3, GL_FLOAT, stride, &draw_cmd.vertex_list[0].p);
		glDrawElements(mode, draw_cmd.index_list.size(), GL_UNSIGNED_SHORT, &draw_cmd.index_list[0]);
	}
}


typedef enum {
	kSolidCube = '1',
	kSolidSphere,
	kSolidTeapot,
	kWireCube,
	kWireSphere,
	kWireTeapot,
	kMeshCount,
} MeshType;

int main()
{
	GLFWwindow *window = nullptr;
	if(!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(kWidth, kHeight, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//set default gl state
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	MeshType mesh_type = kSolidCube;

	// create mesh first
	auto solid_cube_mesh = SolidCubeFactory(1, 1, 1).CreateNormalMesh();
	auto solid_sphere_mesh = SolidSphereFactory(1, 16, 16).CreateNormalMesh();
	auto solid_teapot_mesh = SolidTeapotFactory(0.1).CreateNormalMesh();
	auto wire_cube_mesh = WireCubeFactory(1, 1, 1).CreateSimpleMesh();
	auto wire_sphere_mesh = WireSphereFactory(1, 16, 16).CreateSimpleMesh();
	auto wire_teapot_mesh = WireTeapotFactory(0.1).CreateSimpleMesh();

	//create texture
	unsigned char pixel_data[] = {
		255, 255, 255,
		255, 0, 0,
		0, 255, 0,
		0, 0, 255
	};
	GLuint tex_id = 0;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);


	float rot = 0;
	int running = GL_TRUE;
	while (!glfwWindowShouldClose(window) && running) {
		for(int i = kSolidCube ; i < kMeshCount ; ++i) {
			if(glfwGetKey(window, i) == GLFW_PRESS) {
				mesh_type = (MeshType)i;
			}
		}

		glViewport(0, 0, kWidth, kHeight);
		// OpenGL rendering goes here...
		glClearColor(0, 0, 0, 0);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glShadeModel(GL_SMOOTH);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, kWidth / kHeight, 0.1, 100);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//set cam pos
		gluLookAt(0, 2, 5, 0, 0, 0, 0, 1, 0);

		glRotatef(rot, 0, 1, 0);

		// set light + texture
		if(mesh_type == kSolidCube
			|| mesh_type == kSolidSphere
			|| mesh_type == kSolidTeapot) {
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };		
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);

			//glEnable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_2D);

		} else {
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
		}

		if(mesh_type == kSolidCube) {
			Draw(solid_cube_mesh);

		} else if(mesh_type == kSolidSphere) {
			glRotatef(90, 1, 0, 0);
			Draw(solid_sphere_mesh);

		} else if(mesh_type == kSolidTeapot) {
			glRotatef(-90, 1, 0, 0);
			Draw(solid_teapot_mesh);

		} else if(mesh_type == kWireCube) {
			Draw(wire_cube_mesh);

		} else if(mesh_type == kWireSphere) {
			glRotatef(90, 1, 0, 0);
			Draw(wire_sphere_mesh);

		} else if(mesh_type == kWireTeapot) {
			glRotatef(-90, 1, 0, 0);
			Draw(wire_teapot_mesh);
		}

		// Swap front and back rendering buffers
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		// Check if ESC key was pressed or window was closed
		running = !glfwGetKey(window, GLFW_KEY_ESCAPE);

		rot += 0.03;
	}

	glDeleteTextures(1, &tex_id);

	glfwTerminate();
	return 0;
}