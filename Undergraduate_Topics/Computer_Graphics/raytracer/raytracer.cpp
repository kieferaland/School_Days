#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
//#include "camera.h"
//#include "material.h"
#include <fstream>
//include "glm/glm.hpp"
#include "params_getter.h"

using namespace std;

vec3 random_in_unit_sphere();
params_getter pg;

vec3 color(const ray& r, hitable *world)
{	
	 hit_record rec;
	 if(world->hit(r,0.001,MAXFLOAT, rec))
	 {
		  vec3 target = rec.p + rec.normal;
		  return rec.colour*normalize(target-rec.p);
		  
	 }
	 else
	 {
		  //vec3 unit_direction = normalize(r.direction());
		  //float t = float(0.5)*unit_direction.j+float(1.0);
		  return(vec3(pg.BACK[0],pg.BACK[1],pg.BACK[2]));
	 }
}

vec3 random_in_unit_sphere()
{
	 vec3 p;
	 do
	 {
		  p = float(2.0)*vec3(drand48(),drand48(),drand48())-vec3(1.0,1.0,1.0);
	 }	
	 while (p.squared_length() >= float(1.0));
	 return p;
}

class camera
{
	 public:
		  camera()
		  {
				//set camera according to file params
				lower_left_corner = vec3(0,0,0);
				horizontal = vec3((pg.params["RIGHT"]-pg.params["LEFT"]),0.0,0.0);
				vertical = vec3(0.0,(pg.params["TOP"]-pg.params["BOTTOM"]),0.0);
				origin = vec3(horizontal[0]/2, vertical[1]/2, pg.params["NEAR"]);
		  }
		  ray get_ray(float u, float v)
		  {
				return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
		  }

		  vec3 origin;
		  vec3 lower_left_corner;
		  vec3 horizontal;
		  vec3 vertical;
};

int main (int argc, char* argv[]) 
{
	 ofstream ofs;
	 ofs.open("scene.ppm", ios::trunc);
	 
	 pg.get_params(argc, argv);
	 
	 map<string, float> params = pg.params;
	 map<int,struct sphere_info> spheres = pg.spheres;
	 map<int,struct light_info> lights = pg.lights;
	 int NUM_SPHERES = pg.NUM_SPHERES;
	 int NUM_LIGHTS = pg.NUM_LIGHTS;
	 // resolution width and height respectively in this array
	 int RES[2] = {pg.RES[0], pg.RES[1]};
	 float BACK[3] = {pg.BACK[0], pg.BACK[1], pg.BACK[2]};
	 float AMBIENT[3] = {pg.AMBIENT[0], pg.AMBIENT[1], pg.AMBIENT[2]};
	 string OUTPUT = pg.OUTPUT;
	 int W = RES[0];
	 int H = RES[1];
	 int S = 100;

	 ofs<<"P3\n"<<W<<" "<<H<<"\n255"<<endl;
	
	 //cout<<NEAR<<endl;
	 hitable *objects[NUM_SPHERES];
	 for (int i=0; i<NUM_SPHERES; i++)
	 {
		  //cout<<"making sphere "<<i+1<<endl;
		  objects[i] = new sphere(vec3(si[i].position[0],si[i].position[1],si[i].position[2]), si[i].scaling[0], vec3(si[i].colour[0], si[i].colour[1], si[i].colour[2]));
	 }
	 //objects[0] = new sphere(vec3(0,0,-1), float(0.5));
	 //objects[1] = new sphere(vec3(0, -100.5, -1), float(100));

	 hitable *world = new hitable_list(objects, NUM_SPHERES);
	 camera cam;
	 for (int h = H-1; h>=0; h--){
		  for (int w = 0; w < W; w++)
		  {
				vec3 col(0,0,0);
				
				for (int s = 0; s < S; s++)
				{
					 //anti aliasing, basically randomly selecting some nearby pixels aka filtering
					 //because i didnt take into account true pixel locations
					 float u = float(w + drand48())/float(W);
					 float v = float(h + drand48())/float(H);
					 ray r = cam.get_ray(u,v);
					 
					 vec3 p = r.parameterize(float(2.0));
					 col += color(r,world);
				}
				
				//float u = w/W;
				//float v = h/H;
				//ray r = cam.get_ray(u,v);
				col /= float(S);
				// gamma 2 : means raise the colour to the power of 1/gamma 
				// aka sqrt
				col = vec3(sqrt(col[0]),sqrt(col[1]),sqrt(col[2]));
				int ir = int(255.99*col[0]);
				int ig = int(255.99*col[1]);
				int ib = int(255.99*col[2]);
				
				ofs<<ir<<" "<<ig<<" "<<ib<<endl;
		  }
	 }
	 ofs.close();
	 return 0;
	 // lets see what we've got so far
	 cout<<"\n\nprinting params from raytracer.cpp:\n"<<endl;
	 for (auto param : params)
	 {
		  cout<<param.first<<" : "<<param.second<<endl;
	 }
	 cout<<"RES : ("<<RES[0]<<", "<<RES[1]<<")"<<endl;
	 cout<<"BACK : ("<<BACK[0]<<", "<<BACK[1]<<", "<<BACK[2]<<")"<<endl;
	 cout<<"AMBIENT : ("<<AMBIENT[0]<<", "<<AMBIENT[1]<<", "<<AMBIENT[2]<<")"<<endl;
	 cout<<"OUTPUT : "<<OUTPUT<<endl;
	 cout<<endl<<endl;
	 cout<<"NUM_SPHERES : "<<NUM_SPHERES<<endl;
	 cout<<"NUM_LIGHTS : "<<NUM_LIGHTS<<endl;
	 cout<<endl<<endl;
	 for (int i=0; i<NUM_SPHERES; i++)
	 {
		  cout<<"Sphere "<<i+1<<":\n";
		  cout<<si[i]<<endl;
	 }
	 for (int i=0; i<NUM_LIGHTS; i++)
	 {
		  cout<<"Light "<<i+1<<":\n";
		  cout<<li[i]<<endl;
	 }
}
