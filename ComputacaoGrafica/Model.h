#pragma once
#include <map>
#include <string>
#include <vector>
#include <gl/glut.h>
#include <iostream>
#include <fstream>

using namespace std;

class Model
{
public:
	struct Position	{ float x, y, z; };
	struct Color { float r, g, b; };
	struct Normal { float x, y, z; };

	class Material {
	public:
		float* ambient;
		float* diffuse;
		float* specular;
		GLuint texture;

		Material(float* ambient, float* diffuse, float* specular) {
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->texture = 0;
		}
	};
	class Face {
	public:
		int edge;
		int* vertices;
		int* texcoords;
		int normal;

		Face(int edge, int* vertices, int* texcoords, int normal = -1) {
			this->edge = edge;
			this->vertices = vertices;
			this->texcoords = texcoords;
			this->normal = normal;
		}
	};

	float pos_x, pos_y, pos_z;

	Model();
	~Model();

	void LoadFromFile(const char* fileName);
	vector<float> GetVertexData();
	vector<Face> GetFaceData();
	int GetFaceCount();
	int GetVertexCount();
	void DrawModel();
	void LoadModel(const char* fileName);

private:
	// New
	string prefix;
	vector<Material> materials;
	map<string, int> map_material;
	vector<float*> vertices;
	vector<float*> texcoords;
	vector<float*> normals;
	vector<Face> faces;
	GLuint list;

	void LoadMaterial(const char* fileName);
	void AddFace_3v(string& line);
	void AddFace_3vt(string& line);
	void AddFace_3vn(string& line);
	void AddFace_3vtn(string& line);
	void AddFace_4v(string& line);
	void AddFace_4vt(string& line);
	void AddFace_4vn(string& line);
	void AddFace_4vtn(string& line);

	// Old
	map<string, Color> nMaterialMap;
	vector<float> mVertexData;
	vector<Face> mFacesData;

	void LoadMaterialFile(const char* fileName);
	bool StartWith(string& line, const char* text);
	void AddVertexData(int vIdx, int nIdx, const char* mtl,
		vector<Position>& vertices, vector<Normal>& normals);
};