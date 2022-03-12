#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <gl/glut.h>

using namespace std;

class Model
{
	public:
		class Color {
		public:
			float r;
			float g;
			float b;
			Color() {
				this->r = 0;
				this->g = 0;
				this->b = 0;
			}
			Color(float r, float g, float b) {
				this->r = r;
				this->g = g;
				this->b = b;
			}
		};

		class Normal {
		public:
			float x;
			float y;
			float z;
			Normal() {
				this->x = 0;
				this->y = 0;
				this->z = 0;
			}
		};

		class Vertex {
		public:
			GLfloat x;
			GLfloat y;
			GLfloat z;
			Vertex() {
				this->x = 0;
				this->y = 0;
				this->z = 0;
			}
			Vertex(GLfloat x, GLfloat y, GLfloat z) {
				this->x = x;
				this->y = y;
				this->z = z;
			}
		};

		class Space {
		public:
			float u;
			float v;
			float w;
			Space() {
				this->u = 0;
				this->v = 0;
				this->w = 0;
			}
		};

		class Texture {
		public:
			float u;
			float v;
			float w;
			Texture() {
				this->u = 0;
				this->v = 0;
				this->w = 0;
			}
		};

		class Face {
		public:
			int v0, v1, v2, v3, v4;
			int vt0, vt1, vt2, vt3, vt4;
			int vn0, vn1, vn2, vn3, vn4;
			Face() {
				this->v0 = 0;
				this->v1 = 0;
				this->v2 = 0;
				this->v3 = 0;
				this->v4 = 0;
				this->vt0 = 0;
				this->vt1 = 0;
				this->vt2 = 0;
				this->vt3 = 0;
				this->vt4 = 0;
				this->vn0 = 0;
				this->vn1 = 0;
				this->vn2 = 0;
				this->vn3 = 0;
				this->vn4 = 0;
			}
		};

		Model();
		~Model();

		void LoadFromFile(const char* fileName);
		void DisplayModel(Vertex v, float angle);
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
		Vertex RotateVertex(float angle, Vertex v1, Vertex v2);
};