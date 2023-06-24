#ifndef THREED_ENGINE_CLASSES_
#define THREED_ENGINE_CLASSES_

#define _USE_MATH_DEFINES

#include <GL/glut.h>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;


extern int window_x, window_y;
extern int FOV;
extern const int render_scale;
extern int scaled_x, scaled_y;

typedef struct {
	int x, y, z;
	float a;
	int l;

}player; player P;

typedef struct {
	int r,g,b;
}RGB;

class Texture{
	private:
	FILE* texture_file;
	
	public:
	vector<RGB> colors; 
	int ht,vt;

	Texture(string file_name, int height, int width){
		ifstream texture_file(file_name);
		ht = height;
		vt = width;

		if (texture_file.is_open()){
			RGB color;
			char comma;
			while (texture_file >> color.r >> comma >> color.g >> comma >> color.b >> comma){
				colors.push_back(color);
			}
			texture_file.close();
		}	
	}
};

vector<Texture> texture_list;
void create_textures(){
	texture_list.push_back(Texture("./texture/test.bmp", 16,16));
	texture_list.push_back(Texture("./textures/GRAYBIG_c.bmp", 128,128));
}

#endif