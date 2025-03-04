/**
 * The MIT License
 * 
 * Copyright (c) 2010 Wouter Lindenhof (http://limegarden.net)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include<iostream>
#include<cstdio>
#include<cmath>
#include<vector>
#include<stack>
#include<queue>
#include<map>
#include<sstream>
#include<algorithm>
#include<string>
#include<limits.h>
#include "cyPoint.h"
#include "saveAndRay.h"

using namespace std;
using namespace cy;
Point3f Pe(0,0,0);
#define TOKEN_VERTEX_POS "v"
#define TOKEN_VERTEX_NOR "vn"
#define TOKEN_VERTEX_TEX "vt"
#define TOKEN_FACE "f"
class Object 
{
	public:
		string objectName;
		Object(){}
		virtual Point3f getIntersectionPoint(Ray ray){return Point3f(0,0,0);}
		virtual bool isEyeOutside(Point3f Pe){return false;}
		virtual Color getColor(){return Color(0,0,0);}
		virtual Color lambertShader(Ray myray,Point3f PL){return getColor();}
		virtual Color goochShader(Ray ray,Point3f PL){return getColor();}

		virtual Color phongShader(Ray ray,Point3f PL){return getColor();}

};
Point3f areaOfTriangle(Point3f P0, Point3f P1, Point3f P2)
{
	// returns an area vector
	Point3f v1 = P1 - P0 , v2 = P2 - P0;
	Point3f v0 = v1^v2;
	//double area = v0.Length()/2.0;
	return v0/2.0;
}

/*************************************** OBJECT CLASS **********************************************************************/
class Plane: public Object
{
	public:
		
		Point3f normalVector;
		Point3f origin;
		Color color;
		Plane(){normalVector = Point3f(0,0,1);origin=Point3f(0,0,0);color=Color(1,0.5,0.2);objectName="Plane";}
		Plane(Point3f r, Point3f orig){	normalVector = r; origin=orig;	objectName="Plane";normalVector.Normalize();	}
		Plane(Point3f r, Point3f orig,Color c){	normalVector = r; origin=orig; color = c; objectName="Plane";	normalVector.Normalize();}
		virtual bool isEyeOutside(Point3f Pe)
		{
			double c=(Pe-origin)%(normalVector);
			
			if(c>0)		//Eye is outside the sphere 
				return true;
			else 	return false;
		}
		virtual Color getColor(){	return color;}
		virtual Point3f getIntersectionPoint(Ray ray)
		{
			Point3f ret(-1,-1,-1);
			//cout<<"assuming that eye is outside the sphere \n";
			double t= (normalVector%(origin-ray.origin))/(normalVector%ray.direction);

			//cout<<"value t: "<<t<<" ";
			if (t<0)	return ret;
			ret = ray.origin + t*ray.direction;
			//print(ret);
			return ret;
		}	
		virtual Color lambertShader(Ray myray,Point3f PL)
		{
			Color spColor ;
			Point3f Ph,Pc, nh,nlh;
			//print(interPoint);cout<<endl;
			double cosTheta=0,c;
			double alpha,s,s0,delta;
			Point3f v,r;

			//for(int i=0; i < allObjects.size(); i++)
			//{
			spColor = color;
			Ph=getIntersectionPoint(myray);

			Color ambient_color(0,0,0),diffused_color,finalColor,specular_color(1,1,1);
			// ========================== Lambert and Gooch =========================================	
			Pc=origin;
			diffused_color = color;
			//nh=(Ph - Pc);nh.Normalize();
			nh = normalVector;

			nlh=(PL - Ph);nlh.Normalize();
			cosTheta = nh % nlh;

			// try different values with c and cosTheta
			c = cosTheta;
			//c = (cosTheta+1.0)/2.0;
			//cout<< "cos: "<<(c)<<endl;
			if((c)<0)	//c=0.0;
			{
				//finalColor = ambient_color;
				//return finalColor;
				c=0;
			}
			finalColor = diffused_color*c + ambient_color*(1-c);
			//s=pow(s,alpha);
			//cout<< "c: "<<c<<endl;
			// Gooch shading
			//finalColor = finalColor * color;
			//finalColor.printColor();cout<<endl;
			//========================================== L & G =======================================

			v=Pe-Ph;v.Normalize();
			r= 2*(v%nh)*nh-v ;r.Normalize();
			s = nlh%r,s0=s,delta=1;
			//cout<<"s: "<<s<<endl;
			alpha=4;
			//s=pow(s,alpha);
			//cout<<"v.n and r.n: "<<v%nh<<" "<<r%nh<<endl;
			//if (abs(v%nh - r%nh) >= 0.001 )	s=1;
			if (s<0.0)	s=0;
			else if(s>1.0)	s=1;
			//cout<<"s: "<<s<<endl;
			s=pow(s,alpha);
			//cout<<"s: "<<s<<endl;
			//if (s<0)	s=0;
			finalColor = finalColor*(1-s) + specular_color*(s);
			return finalColor;

		}
		virtual Color goochShader(Ray myray,Point3f PL)
		{
			Color spColor ;
			Point3f Ph,Pc, nh,nlh;
			double cosTheta=0,c;
			double alpha,s,s0,delta;
			Point3f v,r;
			Color ambient_color(0,0,0),diffused_color,finalColor,specular_color(1,1,1);
			//for(int i=0; i < allObjects.size(); i++)
			//{
			spColor = color;
			Ph=getIntersectionPoint(myray);

			// ========================== Lambert and Gooch =========================================	
			diffused_color = color;
			Pc=origin;
			nh=normalVector;
			nlh=(PL - Ph);nlh.Normalize();
			cosTheta = nh % nlh;

			// try different values with c and cosTheta
			c = cosTheta;
			c = (cosTheta+1.0)/2.0;
			if(c<0)	c=0.0;
			finalColor = diffused_color*c + ambient_color*(1-c);
			return finalColor;
		}

		virtual Color phongShader(Ray myray,Point3f PL)
		{
			Color spColor ;
			Point3f Ph,Pc, nh,nlh;
			double cosTheta=0,c;
			double alpha,s,s0,delta;
			Point3f v,r;
			Color ambient_color(0,0,0),diffused_color,finalColor,specular_color(1,1,1);

			Ph=getIntersectionPoint(myray);
			nh=normalVector;
			nlh=(PL - Ph);nlh.Normalize();
			finalColor = lambertShader(myray,PL);

			v=Pe-Ph;v.Normalize();
			r= 2*(v%nh)*nh-v ;r.Normalize();
			s = nlh%r,s0=s,delta=1;
			alpha=4;
			if (s<0.0)	s=0;
			else if(s>1.0)	s=1;
			s=pow(s,alpha);
			finalColor = finalColor*(1-s) + specular_color*(s);
			return finalColor;
		}


};
/*
struct Point2f{
	float x, y;
};
struct Point3f{
	float x, y, z;
};
*/
struct ObjMeshVertex{
	Point3f pos;
	Point2f texcoord;
	Point3f normal;
};

/* This is a triangle, that we can render */
struct ObjMeshFace{
	ObjMeshVertex vertices[3];
};

/* This contains a list of triangles */
struct ObjMesh{
	std::vector<ObjMeshFace> faces;
};

/* Internal structure */
struct _ObjMeshFaceIndex{
	int pos_index[3];
	int tex_index[3];
	int nor_index[3];
};

/* Call this function to load a model, only loads triangulated meshes */
ObjMesh LoadObjMesh(std::string filename){
	ObjMesh myMesh;

	
	std::vector<Point3f>           positions;
	std::vector<Point2f>           texcoords;
	std::vector<Point3f>           normals;
	std::vector<_ObjMeshFaceIndex>  faces;
	/**
	 *      * Load file, parse it
	 *           * Lines beginning with: 
	 *                * '#'  are comments can be ignored
	 *                     * 'v'  are vertices positions (3 floats that can be positive or negative)
	 *                          * 'vt' are vertices texcoords (2 floats that can be positive or negative)
	 *                               * 'vn' are vertices normals   (3 floats that can be positive or negative)
	 *                                    * 'f'  are faces, 3 values that contain 3 values which are separated by / and <space>
	 *                                         */

	std::ifstream filestream;
	filestream.open(filename.c_str());

	std::string line_stream;	// No longer depending on char arrays thanks to: Dale Weiler
	while(std::getline(filestream, line_stream)){	
		std::stringstream str_stream(line_stream);
		std::string type_str;
		str_stream >> type_str;
		if(type_str == TOKEN_VERTEX_POS){
			Point3f pos;
			str_stream >> pos.x >> pos.y >> pos.z;
			positions.push_back(pos);
		}else if(type_str == TOKEN_VERTEX_TEX){
			Point2f tex;
			str_stream >> tex.x >> tex.y;
			texcoords.push_back(tex);
		}else if(type_str == TOKEN_VERTEX_NOR){
			Point3f nor;
			str_stream >> nor.x >> nor.y >> nor.z;
			normals.push_back(nor);
		}else if(type_str == TOKEN_FACE){
			_ObjMeshFaceIndex face_index;
			char interupt;
			for(int i = 0; i < 3; ++i){
				str_stream >> face_index.pos_index[i] >> interupt 
					>> face_index.tex_index[i]  >> interupt 
					>> face_index.nor_index[i];
			}
			faces.push_back(face_index);
		}
	}
	// Explicit closing of the file 
	filestream.close();

	for(size_t i = 0; i < faces.size(); ++i){
		ObjMeshFace face;
		for(size_t j = 0; j < 3; ++j){
			face.vertices[j].pos        = positions[faces[i].pos_index[j] - 1];
			face.vertices[j].texcoord   = texcoords[faces[i].tex_index[j] - 1];
			face.vertices[j].normal     = normals[faces[i].nor_index[j] - 1];
		}
		myMesh.faces.push_back(face);
	}

	return myMesh;
}


/*************************************** OBJECT for .obj files CLASS **********************************************************************/

class GenericObject:public Object
{
	public:
		ObjMesh triangles;
		Color color;
		GenericObject()
		{
			cout<<"constructor str filename\n";
			objectName = "genericObject";
			color = Color(1,0,0);
		}
		GenericObject(ObjMesh tri)
		{
			cout<<"constructor str filename\n";
			triangles = tri;		
			color = Color(1,0,0);
			objectName = "genericObject";
		}
		GenericObject(string fn)
		{

			cout<<"generic filename: "<<fn<<endl;
			triangles = LoadObjMesh(fn);
			cout<<"constructor str filename\n";
			objectName = "genericObject";
			color = Color(1,0,0);
		}

		virtual Color getColor(){	return color;}

		virtual bool isEyeOutside(Point3f Pe)
		{
			int numPlanes = triangles.faces.size();
			for(int i = 0; i < numPlanes; i++ )
			{
				ObjMeshFace triangleFace = triangles.faces[i];
				Plane triPlane( triangleFace.vertices[0].pos , triangleFace.vertices[0].normal  );
				if(!triPlane.isEyeOutside(Pe) )
				{
					return false;
				}
			}

			return true;
		}


		virtual Point3f getIntersectionPoint(Ray ray)
		{
			int numPlanes = triangles.faces.size(), index =-1;
			vector<Point3f> tmpVector;

			for(int i = 0; i < numPlanes; i++ )
			{
				ObjMeshFace triangleFace = triangles.faces[i];
				Plane triPlane( triangleFace.vertices[0].pos , triangleFace.vertices[0].normal  );
				Point3f P0 = triangleFace.vertices[0].pos;
				Point3f P1 = triangleFace.vertices[1].pos;
				Point3f P2 = triangleFace.vertices[2].pos;

				Point3f interPoint = triPlane.getIntersectionPoint(ray) , Ph = interPoint;

				Point3f A0 = areaOfTriangle(P1, P2 , Ph);
				Point3f A1 = areaOfTriangle(P2, P0 , Ph);
				Point3f A2 = areaOfTriangle(P0, P1 , Ph);
				Point3f A  = areaOfTriangle(P0, P1 , P2);

				double w0 = ( A0.x + A0.y + A0.z )/(A.x + A.y + A.z);
				double w1 = ( A1.x + A1.y + A1.z )/(A.x + A.y + A.z);
				double w2 = ( A2.x + A2.y + A2.z )/(A.x + A.y + A.z);

				// if in same plane
				if( w0 + w1 + w2 == 1  )
				{
					// if inside the triangle
					if( (w0 >=0 && w0 <=1) && (w1 >=0 && w1 <=1) &&  (w2 >=0 && w2 <=1) )
						tmpVector.push_back(interPoint);
				}
			}
			double minDist = INT_MAX, len=0.0;
			for(int i = 0; i < tmpVector.size(); i++)
			{
				// if plane doesn't have any intersection point
				if(tmpVector[i].x == -1 && tmpVector[i].y == -1 && tmpVector[i].z == -1)	continue;

				len=(tmpVector[i]-ray.origin).Length();
				if(len < minDist)
				{
					minDist=len;
					index=i;
				}
			}
			if (index == -1)	return Point3f(-1,-1,-1);
			return tmpVector[index];

		}
		virtual Color lambertShader(Ray myray,Point3f PL){return getColor();}
		virtual Color goochShader(Ray ray,Point3f PL){return getColor();}

		virtual Color phongShader(Ray ray,Point3f PL){return getColor();}






};



int main (int argc, char const* argv[])
{
    string str = "diamond.obj";
    ObjMesh mesh = LoadObjMesh(str);
    
    GenericObject cube(str);
    
    return 0;
}
