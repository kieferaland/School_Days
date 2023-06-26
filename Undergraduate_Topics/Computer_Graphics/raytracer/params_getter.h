#include <fstream>
#include <map>
#include <string>
#include <iostream>

using namespace std;
// sphere parameters stored in these class objects
// i initialize 14 because i cant figure out how to name new objects without pre initializing... i'm new to cpp
class sphere_info
{	
	 public:
		  float position[3];
		  float scaling[3];
		  float colour[3];
		  float illumination[4];
		  int specular;

		  friend ostream& operator<<(ostream& os, const sphere_info& _si)
		  {
				return os 	<< "position: "
								<< "("
								<< _si.position[0] << ", "
								<< _si.position[1] << ", "
								<< _si.position[2] << ")\n"
								<< "scaling: "
								<< "("
								<< _si.scaling[0] << ", "
								<< _si.scaling[1] << ", "
								<< _si.scaling[2] << ")\n"
								<< "colour: "
								<< "("
								<< _si.colour[0] << ", "
								<< _si.colour[1] << ", "
								<< _si.colour[2] << ")\n"
								<< "illumination: "
								<< "("
								<< _si.illumination[0] << ", "
								<< _si.illumination[1] << ", "
								<< _si.illumination[2] << ", "
								<< _si.illumination[3] << ")\n"
								<< "specular: "
								<< _si.specular
								<< endl;
		  }

}si[14];

class light_info
{
	 public:
		  float position[3];
		  float intensity[3];

		  friend ostream& operator<<(ostream& os, const light_info& _li)
		  {
				return os 	<< "position: "
								<< "("
								<< _li.position[0] << ", "
								<< _li.position[1] << ", "
								<< _li.position[2] << ")\n"
								<< "intensity: "
								<< "("
								<< _li.intensity[0] << ", "
								<< _li.intensity[1] << ", "
								<< _li.intensity[2] << ")"
								<< endl;
		  }
}li[9];

class params_getter 
{
	 public:
		  // maps to store parameters read from file
		  map<string, float> params;
		  map<int,struct sphere_info> spheres;
		  map<int,struct light_info> lights;
		  int NUM_SPHERES = 0;
		  int NUM_LIGHTS = 0;
		  // resolution width and height respectively in this array
		  int RES[2];
		  float BACK[3];
		  float AMBIENT[3];
		  string OUTPUT;
		  
		  inline int get_params(int argc, char* argv[])
		  {
				// define file for reading parameters from
				ifstream in_file;
				string file_line;

				// line identifier
				int line_num = 0;

				// dictionary to store lines read from file 
				static map<int,string> file_lines;
				
				// once we have the lines in a dictionary we can parse them out into maps and arrays for later lookup
				
				if (argc < 2)
				{
					 return 1;
				}

				cout<<argv[1]<<endl;
				// open file and store text lines in file_lines dictionary
				in_file.open(argv[1]);
				while (getline(in_file, file_line))
				{
					 line_num++;
					 file_lines[line_num] = file_line;
				}
				/*
				// print lines from file_lines dictionary as a test
				cout<<"\nPrinting lines from file_lines dictionary:\n"<<endl;
				for (auto line : file_lines)
				{
					 cout<<line.first<<" : "<<line.second<<endl;
				}
				
				// print number of items in the file_lines dictionary 
				// which should match the number of lines in file
				cout<<"there were "<<line_num<<" lines in this input file.\n"<<endl;
				*/
				// parse lines and store parameters in respective dictionaries for the line header token
				for (auto line: file_lines)
				{		
					 //cout<<"at the start of the FOR loop <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
					 //cout<<line.first<<endl;
					 //cout<<line.second<<endl;
					 
					 // parse the lines delimited by spaces
					 size_t last = 0;
					 size_t next = 0;
					 string delimiter = " ";
					 string token;
					 
					 // for each line
					 while((next = line.second.find(delimiter,last)) != string::npos)
					 {
						  //cout<<"at the start of the WHILE loop ---------------------------------"<<endl;
						  // get current token 
						  token = line.second.substr(last, next-last);
						  //cout<<"starting WHILE with token : "<<token<<endl;
						  // if the first token of a line is OUTPUT we are on the last line
						  if (token.compare("OUTPUT")==0)
						  {
								//cout<<"we didnt come this far to come this far"<<endl;
								last = next+delimiter.length();
								OUTPUT = line.second.substr(last);
								//cout<<"drumroll pls...\n OUTPUT -> "<<OUTPUT<<endl;
								goto after_loop;
						  }
						  // if the first token of a line is not SPHERE or LIGHT
						  else if (token.compare("SPHERE")!=0 && token.compare("LIGHT")!=0)
						  {	
								// if the token is not RES, BACK, or AMBIENT its going into the params map
								if (token.compare("RES")==0)
								{
									 for (int i =0; i<2; i++)
									 {
										  last = next+delimiter.length();
										  next = line.second.find(delimiter,last);
										  token = line.second.substr(last, next-last);
										  RES[i] = stoi(token);
									 }
									 break;
								}
								else if (token.compare("BACK")==0)
								{
									 for (int i=0; i<3; i++)
									 {
										  last = next+delimiter.length();
										  next = line.second.find(delimiter,last);
										  token = line.second.substr(last, next-last);
										  BACK[i] = stof(token);
									 }
									 break;
								}
								else if (token.compare("AMBIENT")==0)
								{
									 for (int i=0; i<3; i++)
									 {
										  last = next+delimiter.length();
										  next = line.second.find(delimiter,last);
										  token = line.second.substr(last, next-last);
										  AMBIENT[i] = stof(token);
									 }
									 break;
								}
								else
								{
									 //cout<<"found an environment param token : "<<token<<endl;
									 params[token];
									 last = next+delimiter.length();
									 //cout<<"end of loop dict entry : token = "<<token<<endl;
									 params[token] = stoi(line.second.substr(last));
									 //cout<<"dictionary entry for "<<token<<" is "<<params[token]<<endl;
								}
						  }
						  // first token of line is SPHERE	
						  else if (token.compare("SPHERE")==0)
						  {	
								// skip the name we don't need it
								last = next+delimiter.length();
								next = line.second.find(delimiter,last);
								for (int i=0; i<3; i++)
								{
									 last = next+delimiter.length();
									 next = line.second.find(delimiter,last);
									 token = line.second.substr(last, next-last);
									 si[NUM_SPHERES].position[i] = stof(token);
								}
								for (int i=0; i<3; i++)
								{
									 last = next+delimiter.length();
									 next = line.second.find(delimiter,last);
									 token = line.second.substr(last, next-last);
									 si[NUM_SPHERES].scaling[i] = stof(token);
								}
								for (int i=0; i<3; i++)
								{
									 last = next+delimiter.length();
									 next = line.second.find(delimiter,last);
									 token = line.second.substr(last, next-last);
									 si[NUM_SPHERES].colour[i] = stof(token);
								}
								for (int i=0; i<4; i++)
								{
									 last = next+delimiter.length();
									 next = line.second.find(delimiter,last);
									 token = line.second.substr(last, next-last);
									 si[NUM_SPHERES].illumination[i] = stof(token);
								}
								last = next+delimiter.length();
								si[NUM_SPHERES].specular = stoi(line.second.substr(last));
								spheres[NUM_SPHERES] = si[NUM_SPHERES];
								NUM_SPHERES++;
								break;
						  }
						  // first token of line is LIGHT
						  else if (token.compare("LIGHT")==0)
						  {	
								// skip the name
								last = next+delimiter.length();
								next = line.second.find(delimiter,last);
								for (int i=0; i<3; i++)
								{
									 last = next+delimiter.length();
									 next = line.second.find(delimiter,last);
									 token = line.second.substr(last, next-last);
									 li[NUM_LIGHTS].position[i] = stof(token);
								}
								for (int i=0; i<3; i++)
								{
									 last = next+delimiter.length();
									 next = line.second.find(delimiter,last);
									 token = line.second.substr(last, next-last);
									 li[NUM_LIGHTS].intensity[i] = stof(token);
								}
								lights[NUM_LIGHTS] = li[NUM_LIGHTS];
								NUM_LIGHTS++;
								break;
						  }
						  // increment the last pointer to after the current delimeter
						  last = next+delimiter.length();

					 }
					 // dont forget the last token
					 // this is wrong as it stands
				}
		  after_loop:
				in_file.close();
				return 0;
				// lets see what we've got so far
				cout<<"\n\nprinting params:\n"<<endl;
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

				//return 0;
		  }
};
