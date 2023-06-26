#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

class vec3
{
    public:
		  vec3(){}
	 	  vec3(float i, float j, float k)
		  {
	 		  this->i = i;
		  	  this->j = j;
			  this->k = k;
		  }

		  inline float x () const {return i;}
		  inline float y () const {return j;}
		  inline float z () const {return k;}
		  inline float r () const {return i;}
		  inline float g () const {return j;}
		  inline float b () const {return k;}
		  
		  inline const vec3& operator + () const {return *this;}
		  inline vec3 operator - () const {return vec3(-i, -j, -k);}
		  
		  inline float operator [] (int i) const 
		  {	
				if (i == 0)
				{

					 return i;
				}
				else if (i == 1)
				{
					 return j;
				}
				else if (i == 2)
				{
					 return k;
				}
				else 
				{
					 cerr << "Error: index out of bounds." << endl; 
					 exit(EXIT_FAILURE);
				}
		  }
		   
		  inline float& operator [] (int i) 
		  {
				if (i == 0)
				{
					 return this->i;
				}
				else if (i == 1)
				{
					 return this->j;
				}
				else if (i == 2)
				{
					 return this->k;
				}
				else 
				{
					 cerr << "Error: index out of bounds." << endl; 
					 exit(EXIT_FAILURE);
				}
		  }

		  inline vec3& operator += (const vec3 &v2);
		  inline vec3& operator -= (const vec3 &v2);
		  inline vec3& operator *= (const vec3 &v2);
		  inline vec3& operator /= (const vec3 &v2);
		  inline vec3& operator *= (const float d);
		  inline vec3& operator /= (const float d);
		  
		  inline float length() const
		  {
				return sqrt(this->i*this->i + this->j*this->j + this->k*this->k);
		  }

		  inline float squared_length() const
		  {
				return this->i*this->i + this->j*this->j + this->k*this->k;
		  }

		  inline void normalize();
		  
		  float i, j, k;
};

inline ostream& operator << (ostream &out, const vec3 &t)
{
	 out << t.i << " " << t.j << " " << t.k;
	 return out;
}

inline istream& operator >> (istream &in, vec3 &t)
{
	 in >> t.i >> t.j >> t.k;
	 return in;
}

inline vec3 operator + (const vec3 &v1, const vec3 &v2)
{
	 return vec3(v1.i+v2.i, v1.j+v2.j, v1.k+v2.k);
}

inline vec3 operator - (const vec3 &v1, const vec3 &v2)
{
	 return vec3(v1.i-v2.i, v1.j-v2.j, v1.k-v2.k);
}

inline vec3 operator * (const vec3 &v1, const vec3 &v2)
{
	 return vec3(v1.i*v2.i, v1.j*v2.j, v1.k*v2.k);
}
 
inline vec3 operator / (const vec3 &v1, const vec3 &v2)
{
	 return vec3(v1.i/v2.i, v1.j/v2.j, v1.k/v2.k);
}

inline vec3 operator * (float d, const vec3 &v)
{
	 return vec3(d*v.i, d*v.j, d*v.k);
}

inline vec3 operator / (const vec3 &v, float d)
{
	 return vec3(v.i/d, v.j/d, v.k/d);
}

inline vec3 operator * (const vec3 &v, float d)
{
	 return vec3(v.i*d, v.j*d, v.k*d);
}

inline float dot(const vec3 &v1, const vec3 &v2)
{
	 return (v1.i*v2.i + v1.j*v2.j + v1.k*v2.k);
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
	 return vec3((v1.j*v2.k - v1.k*v2.j), (v1.k*v2.i - v1.i*v2.k), (v1.i*v2.j - v1.j*v2.i));  
}

inline vec3& vec3::operator += (const vec3 &v)
{
	 this->i += v.i;
	 this->j += v.j;
	 this->k += v.k;
	 return *this;
}

inline vec3& vec3::operator -= (const vec3 &v)
{
	 this->i -= v.i;
	 this->j -= v.j;
	 this->k -= v.k;
	 return *this;
}

inline vec3& vec3::operator *= (const vec3 &v)
{
	 this->i *= v.i;
	 this->j *= v.j;
	 this->k *= v.k;
	 return *this;
}

inline vec3& vec3::operator /= (const vec3 &v)
{
	 this->i /= v.i;
	 this->j /= v.j;
	 this->k /= v.k;
	 return *this;
}

inline vec3& vec3::operator *= (const float d)
{
	 this->i *= d;
	 this->j *= d;
	 this->k *= d;
	 return *this;
}

inline vec3& vec3::operator /= (const float d)
{
	 this->i /= d;
	 this->j /= d;
	 this->k /= d;
	 return *this;
}

inline vec3 normalize(vec3 v)
{
	 return v/v.length();
}
/*
int main()
{	
	 vec3 v1 (3, 5, 9);
	 vec3 v2 (1,1,1);
	 vec3 v6 (2,2,2);

	 cout << "\nv1 = [" << v1.x() << ", " << v1.y() << ", " << v1.z() << "]\n" << endl;
	 cout << "v2 = [" << v2.x() << ", " << v2.y() << ", " << v2.z() << "]\n" << endl;
	 cout << "v6 = [" << v6.x() << ", " << v6.y() << ", " << v6.z() << "]\n" << endl;
	 // test for <<
	 cout << "ostream : " << v1 << "\n" << endl;
	 // test for >>
	 vec3 v3;
	 float arr[3];
	 cout << "enter 3 numbers and be sure to hit <Enter> after each one" << endl;
	 cin >> v3;
	 cout << "the vec3 you created is v3 : [" << v3.i << ", " << v3.j << ", " << v3.k << "]\n" << endl;
	 
	 // test for +
	 vec3 v4 = v1 + v2;
	 cout << "v1 + v2 = [" << v4.i << ", " << v4.j << ", " << v4.k << "]\n" << endl;
	 // test for -
	 vec3 v7 = v1 - v3;
	 cout << "v1 - v3 (you just made v3 a moment ago) = [" << v7.i << ", " << v7.j << ", " << v7.k << "]\n" << endl;
	 // test for - with no lhs operand
	 vec3 v5 = -v1;
	 cout << "-v1 = [" << v5.x() << ", " << v5.y() << ", " << v5.z() << "]\n" << endl; 
	 // test for [] 
	 cout << "v1[2] = " << v1[2] << "\n" << endl;
	 
	 // test for length()
	 cout << "v6 = [" << v6.i << ", " << v6.j << ", " << v6.z() << "]\n" << endl;
	 cout << "v6 has length : " << v6.length() << "\n" << endl;
	 // test for squared_length()
	 cout << "v6 has length_squared :" << v6.squared_length() << "\n" << endl;
	
	 // test for /
	 vec3 v8 = v2/v1;
	 cout << "v2 / v1 = [" << v8.i << ", " << v8.j << ", " << v8.k << "]\n" << endl;

	 // test for *=
	 vec3 v9 (2,2,2);
	 cout << "v9 = [" << v9.i << ", " << v9.j << v9.k << "]\n" << endl;
	 v9 *= v1;
	 cout << "v9*=v1 == [" << v9.i << ", " << v9.j << ", " << v9.k << "]\n" << endl;
    
	 float v10 = dot(v1,v1);
	 cout << "v10 = dot(v1,v1) == " << v10 << endl;

	 vec3 v11 = cross(v1,v1);
	 cout << "v11 = cross(v1,v1) == " << v11 << endl;
	 return 0;
}
*/
