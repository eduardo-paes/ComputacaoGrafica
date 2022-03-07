#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Components.h"

using namespace std;

class Model
{

public:
	Model();
	~Model();

	void LoadFromFile(const char* fileName);
	void DisplayModel(Vertex v);
	bool IsLoaded();

private:
	vector<Vertex> vertices;
	vector<Normal> normals;
	vector<Texture> textures;
	vector<Space> spaces;
	vector<char*> materials;
	map<string, vector<Face>> mapMaterialFace;
	map<string, Color> mapMaterialColor;
	GLuint list;
	bool isModelLoaded;

	void LoadMaterialFile(const char* fileName);
	bool StartWith(string& line, const char* text);
	Color GetObjectColor(string mtlName);
};