#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

// a sphere object i have used this because it is simpler and the resource i was referencing to learn 
// about raytracers used spheres. thats the truth.
class sphere: public hitable
{
	 public: 
		  sphere(){}
		  sphere(vec3 cen, float r, vec3 col): centre(cen), radius(r), colour(col) {};
		  virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
		  vec3 centre;
		  vec3 colour;
		  float radius;
};

bool sphere::hit(const ray&r, float t_min, float t_max, hit_record& rec) const
{
	 // reverse the ray : a line pointing back to the eye from the sphere
	 vec3 eye_loc = r.origin() - centre;
	 // square magnitude of incoming ray
	 float a = dot(r.direction(),r.direction());
	 // get the scalar projection of a ray onto the eye line
	 float b = dot(eye_loc, r.direction());
	 // square magnitude of the vector pointing back to the eye and then subtract radius squared
	 float c = dot(eye_loc, eye_loc) - (radius*radius);
	 // get the discriminant, which will tell us how many roots there are in the parameterized ray when expressed as a function of the sphere
	 float discriminant = (b*b) - (a*c);
	 
	 //discriminant tells us if there are any real roots to the intersection of a ray and the sphere
	 //if the discriminant is negative that means there are only complex roots i think
	 //and i dont really know what that means to be honest. square root of negative one and all that.
	 if (discriminant > 0)
	 {
		  // quadratic equation gives the moment in terms of t when the collision happened
		  float temp = (-b - sqrt(b*b-a*c))/a;
		  if(temp < t_max && temp > t_min)
		  {
				rec.t = temp;
				// the position of the collision
				rec.p = r.parameterize(rec.t);
				// n hat to the collision
				rec.normal = (rec.p-centre)/radius;
				rec.colour = this->colour;
				return true;
		  }
		  temp = (-b + sqrt(b*b - a*c))/a;
		  if(temp < t_max && temp > t_min)
		  {
				rec.t = temp;
				rec.p = r.parameterize(rec.t);
				// n hat to the collision
				rec.normal = (rec.p - centre)/radius;
				return true;
		  }
	 }
	 // ignore rays tangent to the sphere
	 return false;
}

#endif

