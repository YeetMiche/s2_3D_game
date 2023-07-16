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
	int x, y, z; //player position
	float a; //player angle
	float l; //player look
	int cr = 15; //player colission radius
	float sa = 0;  //screen shake amplitude
	bool shooting = false;
	int coins;
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
	texture_list.push_back(Texture("./textures/GRAYBIG_c.bmp", 128,128));		//ID 0
	texture_list.push_back(Texture("./textures/BROWN1_c.bmp", 128,128)); 		//ID 1
	texture_list.push_back(Texture("./textures/STARGR2_c.bmp", 128,128)); 		//ID 2
	texture_list.push_back(Texture("./textures/STONE2_c.bmp", 128,128)); 		//ID 3
	texture_list.push_back(Texture("./textures/BRNBIGC_c.bmp", 128,128)); 		//ID 4
	texture_list.push_back(Texture("./textures/monster_c.bmp", 16,16)); 		//ID 5
	texture_list.push_back(Texture("./textures/monster_hard_c.bmp", 16,16));	//ID 6
	texture_list.push_back(Texture("./textures/weapon_c.bmp", 200,200));		//ID 7
	texture_list.push_back(Texture("./textures/weapon_shoot1_c.bmp", 200,200));	//ID 8
	texture_list.push_back(Texture("./textures/weapon_shoot2_c.bmp", 200,200));	//ID 9
	texture_list.push_back(Texture("./textures/barrier_tape_c.bmp", 128,128));	//ID 10
	texture_list.push_back(Texture("./textures/shopkeep_c.bmp", 16,16));		//ID 11
}

vector<RGB> color_list;
void create_colors(){
	color_list.push_back({30,10,0});		//ID 0 BROWN
	color_list.push_back({255,0,255});		//ID 1 PURPLE
	color_list.push_back({255,0,255});		//ID 2 PURPLE
	color_list.push_back({255,0,255});		//ID 3 PURPLE
	color_list.push_back({255,0,255});		//ID 4 PURPLE
	color_list.push_back({255,0,255});		//ID 5 PURPLE
	color_list.push_back({255,0,255});		//ID 6 PURPLE
	color_list.push_back({255,0,255});		//ID 7 PURPLE
	color_list.push_back({255,0,255});		//ID 8 PURPLE
	color_list.push_back({255,0,255});		//ID 9 PURPLE
	color_list.push_back({255,0,255});		//ID 10 PURPLE
	color_list.push_back({255,0,255});		//ID 11 PURPLE
	color_list.push_back({255,0,255});		//ID 12 PURPLE
	color_list.push_back({255,0,255});		//ID 13 PURPLE
	color_list.push_back({255,0,255});		//ID 14 PURPLE
	color_list.push_back({255,0,255});		//ID 15 PURPLE
	color_list.push_back({255,0,255});		//ID 16 PURPLE
}

#endif