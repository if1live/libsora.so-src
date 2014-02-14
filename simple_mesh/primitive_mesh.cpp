// Ŭnicode please 
#include "primitive_mesh.h"
#include <cassert>
#include <algorithm>
#include <array>
#include "teapot.h"

namespace haruna {;

SolidCubeFactory::SolidCubeFactory(float width, float height, float depth)
	: width_(width), height_(height), depth_(depth)
{
	assert(width > 0 && height > 0 && depth > 0);
}

std::vector<DrawCmdData<Vertex_1P1N1UV>> SolidCubeFactory::CreateNormalMesh()
{
	float width = width_/2;
	float height = height_/2;
	float depth = depth_/2;

	DrawCmdData<Vertex_1P1N1UV> cmd;
	cmd.draw_mode = kDrawTriangles;
	DrawCmdData<Vertex_1P1N1UV>::VertexList &vert_list = cmd.vertex_list;
	std::vector<unsigned short> &index_list = cmd.index_list;

	//normal
	{
		// Front Face
		int baseIndex = vert_list.size();
		Vec3 normal(0, 0, +1);

		Vec2 texCoord1(0, 0);	Vec3 vertex1(-width, -height, depth);
		Vec2 texCoord2(1, 0);	Vec3 vertex2( width, -height, depth);
		Vec2 texCoord3(1, 1);	Vec3 vertex3( width,  height, depth);
		Vec2 texCoord4(0, 1);	Vec3 vertex4(-width,  height, depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}
	{
		// Back Face
		int baseIndex = vert_list.size();
		Vec3 normal(0, 0, -1);

		Vec2 texCoord1(1, 0);	Vec3 vertex1(-width, -height, -depth);
		Vec2 texCoord2(1, 1);	Vec3 vertex2(-width,  height, -depth);
		Vec2 texCoord3(0, 1);	Vec3 vertex3( width,  height, -depth);
		Vec2 texCoord4(0, 0);	Vec3 vertex4( width, -height, -depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	{
		// Top Face
		int baseIndex = vert_list.size();
		Vec3 normal(0, 1, 0);

		Vec2 texCoord1(0, 1);	Vec3 vertex1(-width, height, -depth);
		Vec2 texCoord2(0, 0);	Vec3 vertex2(-width, height,  depth);
		Vec2 texCoord3(1, 0);	Vec3 vertex3( width, height,  depth);
		Vec2 texCoord4(1, 1);	Vec3 vertex4( width, height, -depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	{
		// Bottom Face
		int baseIndex = vert_list.size();
		Vec3 normal(0, -1, 0);

		Vec2 texCoord1(1, 1);	Vec3 vertex1(-width, -height, -depth);
		Vec2 texCoord2(0, 1);	Vec3 vertex2( width, -height, -depth);
		Vec2 texCoord3(0, 0);	Vec3 vertex3( width, -height,  depth);
		Vec2 texCoord4(1, 0);	Vec3 vertex4(-width, -height,  depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	{
		// Right face
		int baseIndex = vert_list.size();
		Vec3 normal(1, 0, 0);

		Vec2 texCoord1(1, 0);	Vec3 vertex1(width, -height, -depth);
		Vec2 texCoord2(1, 1);	Vec3 vertex2(width,  height, -depth);
		Vec2 texCoord3(0, 1);	Vec3 vertex3(width,  height,  depth);
		Vec2 texCoord4(0, 0);	Vec3 vertex4(width, -height,  depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	{
		// Left Face
		int baseIndex = vert_list.size();
		Vec3 normal(-1, 0, 0);

		Vec2 texCoord1(0, 0);	Vec3 vertex1(-width, -height, -depth);
		Vec2 texCoord2(1, 0);	Vec3 vertex2(-width, -height,  depth);
		Vec2 texCoord3(1, 1);	Vec3 vertex3(-width,  height,  depth);
		Vec2 texCoord4(0, 1);	Vec3 vertex4(-width,  height, -depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	std::vector<DrawCmdData<Vertex_1P1N1UV>> data_list;
	data_list.push_back(cmd);
	return data_list;
}

SolidSphereFactory::SolidSphereFactory(float radius, int slices, int stacks)
	: radius_(radius), slices_(slices), stacks_(stacks)
{

}

std::vector<DrawCmdData<Vertex_1P1N1UV>> SolidSphereFactory::CreateNormalMesh()
{
	bool normals = true;
	float nsign = 1.0f;
	float drho = static_cast<float>(kPi / stacks_);
	float dtheta = static_cast<float>(2.0f * kPi / slices_);

	float ds = 1.0f / slices_;
	float dt = 1.0f / stacks_;
	float t = 1.0f; // because loop now runs from 0
	int imin = 0;
	int imax = stacks_;

	typedef DrawCmdData<Vertex_1P1N1UV>::VertexList VertexList;
	typedef DrawCmdData<Vertex_1P1N1UV>::IndexList IndexList;

	DrawCmdData<Vertex_1P1N1UV> cmd;
	cmd.draw_mode = kDrawTriangles;
	VertexList &vert_list = cmd.vertex_list;
	IndexList &index_list = cmd.index_list;

	// draw intermediate stacks as quad strips
	for (int i = imin; i < imax; i++) {
		float rho = i * drho;

		//quad strip로 구성된 vertex 목록 구성하기
		VertexList tmp_vert_list;
		float s = 0.0f;
		for (int j = 0; j <= slices_; j++) {
			float theta = (j == slices_) ? 0.0f : j * dtheta;
			float x = -sin(theta) * sin(rho);
			float y = cos(theta) * sin(rho);
			float z = nsign * cos(rho);

			Vertex_1P1N1UV vert1;
			vert1.n = (Vec3(x * nsign, y * nsign, z * nsign));
			vert1.uv = Vec2(s, t);
			vert1.p = Vec3(x * radius_, y * radius_, z * radius_);
			tmp_vert_list.push_back(vert1);

			x = -sin(theta) * sin(rho + drho);
			y = cos(theta) * sin(rho + drho);
			z = nsign * cos(rho + drho);

			Vertex_1P1N1UV vert2;
			vert2.n = Vec3(x * nsign, y * nsign, z * nsign);
			vert2.uv = Vec2(s, t - dt);
			s += ds;
			vert2.p = Vec3(x * radius_, y * radius_, z * radius_);
			tmp_vert_list.push_back(vert2);
		}

		//DrawCmdData<Vec3> cmd;
		//cmd.draw_mode = kDrawTriangleStrip;
		//quad strip -> triangle strip
		//cmd.vertex_list = vert_list;
		//this->cmd_list_->push_back(cmd);
		int base_vert_list_size = vert_list.size();
		std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), back_inserter(vert_list));

		for(int i = 0 ; i < (int)tmp_vert_list.size()-2 ; ++i) {
			unsigned short idx1, idx2, idx3;
			if(i % 2 == 1) {
				idx1 = i;
				idx2 = i + 2;
				idx3 = i + 1;
			} else {
				idx1 = i;
				idx2 = i + 1;
				idx3 = i + 2;
			}
			index_list.push_back(base_vert_list_size + idx1);
			index_list.push_back(base_vert_list_size + idx2);
			index_list.push_back(base_vert_list_size + idx3);
		}

		t -= dt;
	}
	std::vector<DrawCmdData<Vertex_1P1N1UV>> data_list;
	data_list.push_back(cmd);
	return data_list;
}

WireCubeFactory::WireCubeFactory(float width, float height, float depth)
	: width_(width),
	height_(height),
	depth_(depth)
{
	assert(width > 0 && height > 0 && depth > 0);
}
std::vector<DrawCmdData<Vertex_1P>> WireCubeFactory::CreateSimpleMesh()
{
	float width = width_ / 2.0f;
	float height = height_ / 2.0f;
	float depth = depth_ / 2.0f;

	Vec3 v0(-width, height, -depth);
	Vec3 v1(width, height, -depth);
	Vec3 v2(-width, height, depth);
	Vec3 v3(width, height, depth);

	Vec3 v4(-width, -height, -depth);
	Vec3 v5(width, -height, -depth);
	Vec3 v6(-width, -height, depth);
	Vec3 v7(width, -height, depth);

	std::vector<Vec3> pos_list(8);
	pos_list[0] = v0;
	pos_list[1] = v1;
	pos_list[2] = v2;
	pos_list[3] = v3;
	pos_list[4] = v4;
	pos_list[5] = v5;
	pos_list[6] = v6;
	pos_list[7] = v7;

	DrawCmdData<Vertex_1P> cmd;
	cmd.draw_mode = kDrawLines;
	for(size_t i = 0 ; i < pos_list.size() ; ++i) {
		Vertex_1P vert;
		vert.p = pos_list[i];
		cmd.vertex_list.push_back(vert);
	}

	//GL_LINES용 index list
	std::array<unsigned short, 24> index_list = {
		0,1,	1,3,	2,3,	0,2,
		4,5,	5,7,	6,7,	4,6,
		0,4,	1,5,	2,6,	3,7
	};
	cmd.index_list.resize(index_list.size());
	copy(index_list.begin(), index_list.end(), cmd.index_list.begin());

	std::vector<DrawCmdData<Vertex_1P>> cmd_list;
	cmd_list.push_back(cmd);
	return cmd_list;
}

WireSphereFactory::WireSphereFactory(float radius, int slices, int stacks)
	: radius_(radius), slices_(slices), stacks_(stacks)
{
}

std::vector<DrawCmdData<Vertex_1P>> WireSphereFactory::CreateSimpleMesh()
{
	float nsign = 1.0f;
	float drho = kPi / stacks_;
	float dtheta = 2.0f * kPi / slices_;

	//한방에 그릴수 잇도록하자.
	//vertex list + index로 구성을 변경한다는 소리
	//구성 방법은 gl_lines
	DrawCmdData<Vertex_1P> cmd;
	cmd.draw_mode = kDrawLines;
	std::vector<Vertex_1P> &vert_list = cmd.vertex_list;
	std::vector<unsigned short> &index_list = cmd.index_list;

	// draw stack lines
	for (int i = 1 ; i < stacks_ ; i++) { // stack line at i==stacks-1 was missing here
		float rho = i * drho;

		std::vector<Vertex_1P> tmp_vert_list;
		for (int j = 0; j < slices_; j++) {
			float theta = j * dtheta;
			float x = cos(theta) * sin(rho);
			float y = sin(theta) * sin(rho);
			float z = cos(rho);

			Vertex_1P vert;
			//vert.n[0] = x * nsign;
			//vert.n[1] = y * nsign;
			//vert.n[2] = z * nsign;

			vert.p[0] = x * radius_;
			vert.p[1] = y * radius_;
			vert.p[2] = z * radius_;
			tmp_vert_list.push_back(vert);
		}
		int base_vert_list_size = vert_list.size();
		//copy vertex
		std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), std::back_inserter(vert_list));

		for(int i = 0 ; i < (int)tmp_vert_list.size() ; i++) {
			unsigned short idx1 = base_vert_list_size + i;
			unsigned short idx2 = base_vert_list_size + ((i + 1) % (int)tmp_vert_list.size());
			index_list.push_back(idx1);
			index_list.push_back(idx2);
		}
	}
	// draw slice lines
	for (int j = 0; j < slices_; j++) {
		float theta = j * dtheta;

		std::vector<Vertex_1P> tmp_vert_list;
		for (int i = 0; i <= stacks_; i++) {
			float rho = i * drho;
			float x = cos(theta) * sin(rho);
			float y = sin(theta) * sin(rho);
			float z = cos(rho);

			Vertex_1P vert;
			//vert.n[0] = x * nsign;
			//vert.n[1] = y * nsign;
			//vert.n[2] = z * nsign;

			vert.p[0] = x * radius_;
			vert.p[1] = y * radius_;
			vert.p[2] = z * radius_;
			tmp_vert_list.push_back(vert);
		}
		int base_vert_list_size = vert_list.size();
		std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), std::back_inserter(vert_list));

		for(int i = 0 ; i < (int)tmp_vert_list.size() - 1 ; i++) {
			unsigned short idx1 = base_vert_list_size + i;
			unsigned short idx2 = base_vert_list_size + ((i + 1) % (int)tmp_vert_list.size());
			index_list.push_back(idx1);
			index_list.push_back(idx2);
		}
	}

	std::vector<DrawCmdData<Vertex_1P>> cmd_list;
	cmd_list.push_back(cmd);
	return cmd_list;
}

std::vector<DrawCmdData<Vertex_1P1N1UV>> SolidTeapotFactory::CreateNormalMesh()
{
	DrawCmdData<Vertex_1P1N1UV> cmd;
	cmd.draw_mode = kDrawTriangles;
	cmd.vertex_list.resize(NUM_TEAPOT_OBJECT_VERTEX);
	for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
		Vertex_1P1N1UV &vert = cmd.vertex_list[i];
		vert.p[0] = teapotVertices[i*3+0] * size_;
		vert.p[1] = teapotVertices[i*3+1] * size_;
		vert.p[2] = teapotVertices[i*3+2] * size_;

		vert.uv[0] = teapotTexCoords[i*2+0];
		vert.uv[1] = teapotTexCoords[i*2+1];

		vert.n[0] = teapotNormals[i*3+0];
		vert.n[1] = teapotNormals[i*3+1];
		vert.n[2] = teapotNormals[i*3+2];
	}

	cmd.index_list.resize(NUM_TEAPOT_OBJECT_INDEX);
	memcpy(&cmd.index_list[0], teapotIndices, sizeof(teapotIndices));

	std::vector<DrawCmdData<Vertex_1P1N1UV>> cmd_list;
	cmd_list.push_back(cmd);
	return cmd_list;
}
std::vector<DrawCmdData<Vertex_1P>> WireTeapotFactory::CreateSimpleMesh()
{
	DrawCmdData<Vertex_1P> cmd;
	cmd.draw_mode = kDrawLines;
	cmd.vertex_list.resize(NUM_TEAPOT_OBJECT_VERTEX);
	for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
		Vertex_1P &vert = cmd.vertex_list[i];
		vert.p[0] = teapotVertices[i*3+0] * size_;
		vert.p[1] = teapotVertices[i*3+1] * size_;
		vert.p[2] = teapotVertices[i*3+2] * size_;
	}

	for(int i = 0 ; i < NUM_TEAPOT_OBJECT_INDEX / 3 ; i++) {
		unsigned short idx_1 = teapotIndices[i*3+0];
		unsigned short idx_2 = teapotIndices[i*3+1];
		unsigned short idx_3 = teapotIndices[i*3+2];
		cmd.index_list.push_back(idx_1);
		cmd.index_list.push_back(idx_2);
		cmd.index_list.push_back(idx_2);
		cmd.index_list.push_back(idx_3);
		cmd.index_list.push_back(idx_3);
		cmd.index_list.push_back(idx_1);
	}
	std::vector<DrawCmdData<Vertex_1P>> cmd_list;
	cmd_list.push_back(cmd);
	return cmd_list;
}

}	//namespace haruna