#include "Model.h"

#pragma region Constructors

Model::Model() 
{ 
    vector<Vertex> vertices = vector<Vertex>();
    vector<Normal> normals = vector<Normal>();
    vector<Texture> textures = vector<Texture>();
    vector<Space> spaces = vector<Space>();
	isModelLoaded = false;
}

Model::~Model() { }

#pragma endregion

#pragma region Auxiliary_Methods

int CountChar(string& str, char ch) {
	int c = 0;
	int length = str.length() - 1;
	for (int i = 0; i < length; i++) {
		if (str[i] == ch)
			c++;
	}
	return c;
}

bool HasDoubleSlash(string& str) {
	int length = str.length() - 2;
	for (int i = 0; i < length; i++) {
		if (str[i] == '/' && str[i + 1] == '/')
			return true;
	}
	return false;
}

bool Model::StartWith(string& line, const char* text) {
	size_t textLen = strlen(text);
	if (line.size() < textLen) {
		return false;
	}
	for (size_t i = 0; i < textLen; i++)
	{
		if (line[i] == text[i]) continue;
		else return false;
	}
	return true;
}

string GetConcatString(char* text1, string text2) {
	ostringstream ss;
	ss << text1 << text2;
	return ss.str();
}

#pragma endregion

#pragma region File_Loaders

void Model::LoadFromFile(const char* fileName) {
	ifstream file(fileName);
	if (file) {
		char currentMtlName[100];
		string line;
		while (getline(file, line)) {

            // Materials that describe the visual aspects of the polygons are stored in external .mtl files. 
            // More than one external MTL material file may be referenced from within the OBJ file. 
            // The .mtl file may contain one or more named material definitions.
			if (StartWith(line, "mtllib"))
			{
				char mtlFileName[100];
				(void)sscanf_s(line.c_str(), "mtllib %s", mtlFileName, sizeof(mtlFileName));
				LoadMaterialFile(mtlFileName);
			}

            // NORMAL: List of vertex normals in (x,y,z) form; normals might not be unit vectors.
			else if (StartWith(line, "vn")) {
				Normal n = Normal();
				(void)sscanf_s(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
				normals.push_back(n);
			}

            // TEXTURE: List of texture coordinates, in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.
            else if (StartWith(line, "vt")) {
                Texture t = Texture();
                int spaces = CountChar(line, ' ');

                if (spaces == 1)
                    (void)sscanf_s(line.c_str(), "vt %f", &t.u);
                else if (spaces == 2)
                    (void)sscanf_s(line.c_str(), "vt %f %f", &t.u, &t.v);
                else
                    (void)sscanf_s(line.c_str(), "vt %f %f %f", &t.u, &t.v, &t.w);

                textures.push_back(t);
            }

            // SPACE: Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement.
            else if (StartWith(line, "vp")) {
                Space s = Space();
                int blankSpace = CountChar(line, ' ');

                if (blankSpace == 1)
                    (void)sscanf_s(line.c_str(), "vp %f", &s.u);
                else if (blankSpace == 2)
                    (void)sscanf_s(line.c_str(), "vp %f %f", &s.u, &s.v);
                else
                    (void)sscanf_s(line.c_str(), "vp %f %f %f", &s.u, &s.v, &s.w);

				spaces.push_back(s);
            }

            // VERTEX: List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.
			else if (StartWith(line, "v")) {
				Vertex v = Vertex();
				(void)sscanf_s(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
				vertices.push_back(v);
			}

            // MATERIAL_NAME: This tag specifies the material name for the element following it. 
            // The material name matches a named material definition in an external .mtl file.
			else if (StartWith(line, "usemtl")) {
				(void)sscanf_s(line.c_str(), "usemtl %s", currentMtlName, sizeof(currentMtlName));
				materials.push_back(currentMtlName);
			}

            // FACES: Polygonal face elements
			else if (StartWith(line, "f")) {
				int spaces = CountChar(line, ' ');
				int slashes = CountChar(line, '/');

				Face val = Face();

				// - Slashes = 0 and Spaces = 3 or 4
				if (slashes == 0) {
					if (spaces == 3) {
						(void)sscanf_s(line.c_str(), "f %d %d %d", &val.v0, &val.v1, &val.v2);
					}
					else if (spaces == 4) {
						(void)sscanf_s(line.c_str(), "f %d %d %d %d", &val.v0, &val.v1, &val.v2, &val.v3);
					}
					else if (spaces == 5) {
						(void)sscanf_s(line.c_str(), "f %d %d %d %d", &val.v0, &val.v1, &val.v2, &val.v3, &val.v4);
					}
				}

				// - Slashes = Spaces and Spaces = 3 or 4
				else if (slashes == spaces) {
					if (slashes == 3) {
						(void)sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &val.v0, &val.vt0, &val.v1, &val.vt1, &val.v2, &val.vt2);
					}
					else if (slashes == 4) {
						(void)sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &val.v0, &val.vt0, &val.v1, &val.vt1, &val.v2, &val.vt2, &val.v3, &val.vt3);
					}
					else if (slashes == 5) {
						(void)sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d %d/%d", &val.v0, &val.vt0, &val.v1, &val.vt1, &val.v2, &val.vt2, &val.v3, &val.vt3, &val.v4, &val.vt4);
					}
				}

				else if (slashes == spaces * 2) {
					// - Has Double Slash and Spaces = 3 or 4
					if (HasDoubleSlash(line)) {
						if (slashes == 6) {
							(void)sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &val.v0, &val.vn0, &val.v1, &val.vn1, &val.v2, &val.vn2);
						}
						else if (slashes == 8) {
							(void)sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &val.v0, &val.vn0, &val.v1, &val.vn1, &val.v2, &val.vn2, &val.v3, &val.vn3);
						}
						else if (slashes == 10) {
							(void)sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d %d//%d", &val.v0, &val.vn0, &val.v1, &val.vn1, &val.v2, &val.vn2, &val.v3, &val.vn3, &val.v4, &val.vn4);
						}
					}

					// - Hasn't Double Slash and Spaces = 3 or 4
					else {
						if (slashes == 6) {
							(void)sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &val.v0, &val.vt0, &val.vn0, &val.v1, &val.vt1, &val.vn1, &val.v2, &val.vt2, &val.vn2);
						}
						else if (slashes == 8) {
							(void)sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &val.v0, &val.vt0, &val.vn0, &val.v1, &val.vt1, &val.vn1, &val.v2, &val.vt2, &val.vn2, &val.v3, &val.vt3, &val.vn3);
						}
						else if (slashes == 10) {
							(void)sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &val.v0, &val.vt0, &val.vn0, &val.v1, &val.vt1, &val.vn1, &val.v2, &val.vt2, &val.vn2, &val.v3, &val.vt3, &val.vn3, &val.v4, &val.vt4, &val.vn4);
						}
					}
				}

				// Insert Face in the current Material
				mapMaterialFace[currentMtlName].push_back(val);
			}

            // LINES: Line element
            else if (StartWith(line, "l")) {
                // TODO: Check how to implement it.
            }
		}

		isModelLoaded = true;
		DisplayModel(Vertex());
	}
	else {
		cout << "Model file loading failed." << endl;
		isModelLoaded = false;
	}
}

void Model::DisplayModel(Vertex v) {
	Normal norm;
	Vertex v1, v2, v3;
	string nameMatMap;
	Color color;
	char* mtlName;

	for (auto values = mapMaterialFace.begin(); values != mapMaterialFace.end(); ++values) {

		/*mtlName = (char*)values->first.c_str();
		nameMatMap = GetConcatString(mtlName, "_Ks");
		color = mapMaterialColor[nameMatMap];
		glColor3ub(color.r, color.g, color.b);*/

		glColor3ub(rand() % 255, rand() % 255, rand() % 255);

		for (Face tmp : values->second) {
			if (tmp.v0 == 0 && tmp.v1 == 0 && tmp.v2 == 0) continue;

			v1 = vertices[tmp.v0 == 0 ? tmp.v0 : tmp.v0 - 1];
			v2 = vertices[tmp.v1 == 0 ? tmp.v1 : tmp.v1 - 1];
			v3 = vertices[tmp.v2 == 0 ? tmp.v2 : tmp.v2 - 1];
			norm = normals[tmp.vn0 == 0 ? tmp.vn0 : tmp.vn0 - 1];

			glBegin(GL_TRIANGLES);
			glVertex3f(v1.x + v.x, v1.y + v.y, v1.z + v.z);
			glVertex3f(v2.x + v.x, v2.y + v.y, v2.z + v.z);
			glVertex3f(v3.x + v.x, v3.y + v.y, v3.z + v.z);
			glNormal3f(norm.x, norm.y, norm.z);
			glEnd();
		}
	}
}

bool Model::IsLoaded() {
	return isModelLoaded;
}

void Model::LoadMaterialFile(const char* fileName) {
	ostringstream ss;
	ss << "./Models/" << fileName;
	string fullFileName = ss.str();

	ifstream file(fullFileName);
	if (file) {
		string line, nameMatMap;
		Color color;
		char mtlName[100];
		while (getline(file, line)) {

			// Get Material Name
			if (StartWith(line, "newmtl")) {
				(void)sscanf_s(line.c_str(), "newmtl %s", mtlName, sizeof(mtlName));
			}

			// Ambient Color
			else if (StartWith(line, "Ka")) {
				nameMatMap = GetConcatString(mtlName, "_Ka");
				sscanf_s(line.c_str(), "Ka %f %f %f", &color.r, &color.g, &color.b);
				mapMaterialColor[nameMatMap] = color;
			}

			// Diffuse Color
			else if (StartWith(line, "Kd")) {
				nameMatMap = GetConcatString(mtlName, "_Kd");
				sscanf_s(line.c_str(), "Kd %f %f %f", &color.r, &color.g, &color.b);
				mapMaterialColor[nameMatMap] = color;
			}

			// Specular Color
			else if (StartWith(line, "Ks")) {
				nameMatMap = GetConcatString(mtlName, "_Ks");
				sscanf_s(line.c_str(), "Ks %f %f %f", &color.r, &color.g, &color.b);
				mapMaterialColor[nameMatMap] = color;
			}

			// Specular Exponent
			else if (StartWith(line, "Ns")) {
				nameMatMap = GetConcatString(mtlName, "_Ns");
				sscanf_s(line.c_str(), "Ns %f %f %f", &color.r, &color.g, &color.b);
				mapMaterialColor[nameMatMap] = color;
			}

			// Transparency (can be Tr or d)
			else if (StartWith(line, "Tr")) {
				float tr;
				sscanf_s(line.c_str(), "Tr %f", &tr);
			}

			// Transparency (can be Tr or d)
			else if (StartWith(line, "d")) {
				float d;
				sscanf_s(line.c_str(), "d %f", &d);
			}

			// Optical Density
			else if (StartWith(line, "Ni")) {
				float ni;
				sscanf_s(line.c_str(), "Ni %f", &ni);
			}
		}
	}
	else {
		cout << "Material file loading failed." << endl;
	}
}

#pragma endregion