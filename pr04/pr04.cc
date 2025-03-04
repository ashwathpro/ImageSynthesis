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
#include<limits.h>
#include "ObjectsDefine.h"
#define print(p) printf("(%f,%f,%f) \n",p.x,p.y,p.z);
#define pb(a) push_back(a)
class SpotLight
{
    public:
      Point3f source,direction;
      double angle;

      SpotLight(){  source=Point3f(0,0,0),direction=Point3f(0,0,0);angle = 30;}
      SpotLight(Point3f src,Point3f dir,double an){  source=src,direction=dir;angle = an;}
      SpotLight(Point3f src,Point3f dir,float an){  source=src,direction=dir;angle = an;}


};

Point3f Pe(0,0,0);      //camera or eye position
SpotLight spotLight(Point3f(10,-2,0),Point3f(0,0,1),60.0/180.0);

//Point3f PL = spotLight.source;
Point3f DirectionLight(0,-1,0);

/*** READ PPM ******/
int projectionImageWidth, projectionImageHeight, maxcolor;
int bumpImageWidth, bumpImageHeight, bumpmaxcolor;

unsigned char *pixmap, *bumpMap;
//void readPPM(char *argv[])

void readPPM()
{ 
	  int ch, bit, comment;
	    FILE *fp;
	    fp=fopen("texture1.ppm","r");					//open PPM file for reading
	    //fp=fopen("red.ppm","r");					//open PPM file for reading
	    if(fp == NULL)
	    {
		    printf("\n File Error!\n");
		    exit(0);
	    }
	    char magic[10];	
	    fscanf(fp, "%s", magic);
	    if(magic[0]!='P'||magic[1]!='6')			//check the image format
	    {
		    printf("\n Magic file for the input file is not P6\n");
		    exit(0);
	    }
	    ch=fgetc(fp);						//check for comment
	    do {
		    if (ch == '#');
		    ch = fgetc(fp);
	    } while (ch == '\n');
	    ungetc(ch, fp);
	    ch= fgetc(fp);
	    while (ch == '#') 
	    {
		    while (fgetc(fp) != '\n') ;
		    ch = fgetc(fp);
	    }
	    ungetc(ch, fp);
	    fscanf (fp, "%d %d %d", &projectionImageWidth, &projectionImageHeight, &maxcolor);	//read image size information and maxcolor
	    fgetc(fp);
	    pixmap = new unsigned char[projectionImageWidth * projectionImageHeight * 3];         // Dynamic memory allocation
	    int y, x, pixel;
	    unsigned char red, green, blue;
	    for(y = projectionImageHeight-1; y >= 0; y--) 
	    {
		    for(x = 0; x < projectionImageWidth; x++) 
		    {
			    fscanf(fp, "%c%c%c", &red, &green, &blue);
			    pixel = (y * projectionImageWidth + x) * 3; 
			    pixmap[pixel] = red;
			    pixel++;
			    pixmap[pixel] = green;
			    pixel++;
			    pixmap[pixel] = blue;
		    }
	    }
	    fclose(fp);
	    //  }  								// Close if

}									// End of the function
void readPPMBumpMap()
{ 
	  int ch, bit, comment;
	    FILE *fp;
	    fp=fopen("tex1.ppm","r");					//open PPM file for reading
	    //fp=fopen("red.ppm","r");					//open PPM file for reading
	    if(fp == NULL)
	    {
		    printf("\n File Error!\n");
		    exit(0);
	    }
	    char magic[10];	
	    fscanf(fp, "%s", magic);
	    if(magic[0]!='P'||magic[1]!='6')			//check the image format
	    {
		    printf("\n Magic file for the input file is not P6\n");
		    exit(0);
	    }
	    ch=fgetc(fp);						//check for comment
	    do {
		    if (ch == '#');
		    ch = fgetc(fp);
	    } while (ch == '\n');
	    ungetc(ch, fp);
	    ch= fgetc(fp);
	    while (ch == '#') 
	    {
		    while (fgetc(fp) != '\n') ;
		    ch = fgetc(fp);
	    }
	    ungetc(ch, fp);
	    fscanf (fp, "%d %d %d", &bumpImageWidth, &bumpImageHeight, &bumpmaxcolor);	//read image size information and maxcolor
	    fgetc(fp);
	    bumpMap= new unsigned char[bumpImageWidth * bumpImageHeight * 3];         // Dynamic memory allocation
	    int y, x, pixel;
	    unsigned char red, green, blue;
	    for(y = bumpImageHeight-1; y >= 0; y--) 
	    {
		    for(x = 0; x < bumpImageWidth; x++) 
		    {
			    fscanf(fp, "%c%c%c", &red, &green, &blue);
			    pixel = (y * bumpImageWidth + x) * 3; 
			    bumpMap[pixel] = red;
			    pixel++;
			    bumpMap[pixel] = green;
			    pixel++;
			    bumpMap[pixel] = blue;
		    }
	    }
	    fclose(fp);
	    //  }  								// Close if

	    }									// End of the function
/********************************************* MAIN ****************************************************************/
/*
void printVector(obj_vector *v)
{
	printf("%.2f,", v->e[0] );
	printf("%.2f,", v->e[1] );
	printf("%.2f ", v->e[2] );
}
*/

struct FacePoint
{
	int faceindex;
	Point3f point;
};

int main (int argc, char const* argv[])
{
	int antiAliasing=0;

	
        bool spotlightEnabled=false;
	
	int Xmax = 500;
	int Ymax = 500;
	int n=Ymax*Xmax,dpi=72;

	readPPM();
	readPPMBumpMap();
	
	RGBType *pixels= new RGBType[n];
	int index=0,M=2,N=2; 
	int Sx=10,winIndex=0;
	int Sy=(Sx*Ymax)/Xmax;
	float x,y;
	
	Point3f Vview(0,0,1),Vup(0,1,0);	// point the view vector to focus on a particular point from Pe
	//Vview = Vview - Pe;
	Vview.Normalize();
	//Vup.Normalize();
	Point3f n2=Vview,n0=n2^Vup;
	n0.Normalize();
	Point3f n1=n0^n2;
	n1.Normalize();
	int d=3;
	Point3f npe=Pe,Pcenter=Pe+d*n2;
	npe.Normalize();//print(npe);
	Point3f P00=Pcenter-(Sx/2)*n0-(Sy/2)*n1,Pp;
	//vector<Sphere> allSpheres;
	//allSpheres.pb(sphere1);
	
	//string genericObjFileName="cube1.obj";
	//printf("%s", genericObjFileName.c_str());
	


	string genericObjFileName="cube_00.obj";
	genericObjFileName = "ico.obj";
	 //char *objfilename = "tetrahedron.obj";

/*
	objLoader *objData = new objLoader();
	int someint= objData->load(objfilename);
	printf("Number of faces: %i\n", objData->faceCount);
	for(int i=0; i<objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];
		printf(" face ");
		for(int j=0; j<3; j++)
		{
			printVector(objData->vertexList[ o->vertex_index[j] ]);
		}
		printf("\n");
	}

*/

	Sphere sphere1(Point3f(0,0,20),10, Color(1,0.8,0),1);
	Sphere sphere2(Point3f(-3,-2,10),3, Color(1,0.2,0.7),2);
	//Sphere sphere3(Point3f(-1,5,8),3, Color(0.1,0.5,1),3);
	//Sphere sphere4(Point3f(1,1,6),3, Color(0,0.5,1),4);
	
	Plane plane1(Point3f(0,-1,0), Point3f(0,130,0), Color(.3,0,.3));
	Plane plane2(Point3f(-1,0,0), Point3f(130,0,0), Color(.3,1,.3));
	Plane plane3(Point3f(0,0,-1), Point3f(0,0,140), Color(.3,1,1));
	Plane plane4(Point3f(0,0,1), Point3f(0,0,-130), Color(0,1,0));
	Plane plane5(Point3f(0,1,0), Point3f(0,-130,0), Color(1,0,0));
	Plane plane6(Point3f(1,0,0), Point3f(-130,0,0), Color(1,1,0));

	//cout<<"debug/////";
	GenericObject cube(genericObjFileName);

	//ObjMesh triMesh= LoadObjMesh(genericObjFileName);

	//GenericObject cube1(triMesh);
	//GenericObject cube1(genericObjFileName);
	//cout<<"debug/////";

	vector<Object*> allObjects;
	//allObjects.push_back(dynamic_cast<Object*>(&sphere1));
	//allObjects.push_back(dynamic_cast<Object*>(&sphere2));
	//allObjects.push_back(dynamic_cast<Object*>(&sphere3));
	//allObjects.push_back(dynamic_cast<Object*>(&sphere4));
	
	allObjects.push_back(dynamic_cast<Object*>(&plane1));
	allObjects.push_back(dynamic_cast<Object*>(&plane2));
	allObjects.push_back(dynamic_cast<Object*>(&plane3));
	allObjects.push_back(dynamic_cast<Object*>(&plane4));
	allObjects.push_back(dynamic_cast<Object*>(&plane5));
	allObjects.push_back(dynamic_cast<Object*>(&plane6));

	allObjects.push_back(dynamic_cast<Object*>(&cube));
	//*/
	Ray myray(Pe,npe);
	int No=allObjects.size();	
	vector<Point3f > myinter;
	float rnd;


				//printf("psi: %f 
        Point3f PL;
	PL = spotLight.source;

	// CHECK IF YOU'RE INSIDE ANY OF THE SPHERES ///////////////////////////
	for (int i = 0; i < allObjects.size(); i++)
	{	
          if(!allObjects[i]->isEyeOutside(Pe) )
		{	cout<<"Eye is inside the object \n";return 1;}
	}
	cout<<"EEye and light are outside all the object \n";
	
	
	
        
	
	
	
	vector<Point3f> lights;
        lights.pb(Point3f(10,24,30));
        lights.pb(Point3f(-10,24,30));
        lights.pb(Point3f(0,24,0));

        double alpha0= cos(spotLight.angle);
        int softShadowFlag=0;
	vector <Point3f> tmp;
	//tmp.resize(2);
	Color finalColor(0,0,0);
        Color TotalfinalColor(0,0,0);
        double intensity=1.0/(lights.size());
	Point3f interPoint(0,0,0);
	int spNum=0;
        double alpha=0;
        bool isOnePicture =true;
	for (int I = 0; I < Xmax; I++)
	{
		for (int J = 0; J < Ymax; J++)
		{
			index=J*Xmax + I;
			// shooting rays from center of the pizel

			x=(I+0.5)/Xmax;
			y=(J+0.5)/Ymax;

			Pp=P00+(x*Sx)*n0+(y*Sy)*n1;	//Direction to shoot the ray
			myray=Ray(Pe, Pp);	// This is the ray that we will shoot from camera to find out the color at pixel x,y
			tmp.clear();
			for (int i = 0; i < allObjects.size(); i++)
			{
				interPoint=allObjects[i]->getIntersectionPoint(myray);
				//if((allObjects[winIndex]->objectName=="genericObject"))
				//	cout<<"intersection point with plane : "; print(interPoint);
				//myinter.pb(interPoint);
				//if(interPoint.x!=-1)	
				tmp.pb(interPoint);	//get intersection points
			}
			//cout<<"tmpsize: "<<tmp.size();cout<<endl;
			//if(interPoint.x == -1)

			//find which point wins
			pixels[index].r=0;
			pixels[index].g=0;
			pixels[index].b=0;

			winIndex= findWinningPointIndex(tmp , Pe);
			if(winIndex==-1)
				continue;
			//assign that index object's color to the image
			//shader equatoins:
			Point3f shadowInter,rayStart = tmp[winIndex], interSectionPoint = tmp[winIndex];
			int flag=0;
                        Point3f NH ;
                        finalColor = Color(0,0,0);



                        

                       TotalfinalColor = Color(0,0,0);


                        int numLights = lights.size();
			//print(PL);
			finalColor = Color(0,0,0);
			Ray jujuRay(rayStart , PL-rayStart);

			flag=0;
			tmp.clear();
			double shadowDist=0.0,distToPL=(PL-rayStart).Length(),ratio=1.0,o=1,maxDark=1;
			int power=1;
			double cDirect;
			//*/ FOR SHARP SHADOWS 
			// * get all other intersection points with jujuRay except for the object itself (winIndex)
			for(int j=0;j<allObjects.size();j++)
			{
				if(j==winIndex)	continue;
				// for soft shadows, find the distance between the intersection points also
				if(allObjects[j]->objectName=="Sphere" )	
				{
					shadowDist += (((Sphere*)allObjects[j])->getTwoDelta(jujuRay))/(2*( ((Sphere*)allObjects[j])->radius)) ;
					shadowDist = min(maxDark, shadowDist);
				}
                                else if(allObjects[j]->objectName=="genericObject" )	
				{
					shadowDist += (((GenericObject*)allObjects[j])->twoDist(jujuRay))/5.0;
                                        shadowDist = min(maxDark, shadowDist);
                                }

				shadowInter = allObjects[j]->getIntersectionPoint(jujuRay);
				tmp.pb(shadowInter);

			}
			int mywinIndex= findWinningPointIndex(tmp , rayStart);
			if(mywinIndex !=-1 && allObjects[mywinIndex]->objectName=="Sphere")	
			{
				NH  = (tmp[mywinIndex] - ((Sphere*)(allObjects[mywinIndex]))->center   );
			}
			//*
			else if(mywinIndex !=-1 && allObjects[mywinIndex]->objectName=="Plane")	
			{
				NH = (-1)*(((Plane*)(allObjects[mywinIndex]))->normalVector);
			}


			NH.Normalize();
			DirectionLight.Normalize();
			cDirect = DirectionLight%NH;                         // for directional light control value
			//cout<<cDirect<<endl;
			if(cDirect < 0.0 )
			{
				//cout<<cDirect<<endl;
				cDirect = 1;
			}
			else {
				//cout<<cDirect<<endl;
				cDirect = 0;
			}


			// */
			// uncomment the following line for normal point light
			cDirect = 0;
			if(mywinIndex !=-1)// && tmp[mywinIndex]!=rayStart)
			{
				// if distance between startRay and the point tmp[mywinIndex] < dist b/n startRay and PL then black is the color -> continue
				double pointToInter = (rayStart-tmp[mywinIndex]).Length(), pointToLight = ( rayStart-PL).Length();

				if( pointToInter < pointToLight)
				{
					//ratio =  1 - (shadowDist/distToPL)  ;

					ratio =  1 - pow(shadowDist,power)  ;
					// comment the following line to have sharp shadows
					softShadowFlag = 1;


				}
			}
			// FOR Soft shadows
			Color shadowColor(ratio,ratio,ratio);
			//shadowColor=shadowColor*ratio;


			alpha = ((rayStart - spotLight.source)%spotLight.direction)/((rayStart - spotLight.source).Length()*(spotLight.direction).Length());
			if(alpha >alpha0) alpha=1;
			else alpha = 0;

			if(allObjects[winIndex]->objectName=="Sphere")	
			{
				if(spotlightEnabled)
					finalColor = finalColor+ (((Sphere*)allObjects[winIndex])->phongShader(myray,PL))*alpha;
				else
                                {
                                  double X,Y,S0=10,S1=10,Z;
                                  S0=500;
                                  S1=S0;


                                  interSectionPoint =( interSectionPoint - ((Sphere*)allObjects[winIndex])->center);
                                  interSectionPoint = interSectionPoint/((Sphere*)allObjects[winIndex])->radius;

                                  X = interSectionPoint%Point3f(1,0,0);
                                  Y = interSectionPoint%Point3f(0,1,0);
                                  Z = interSectionPoint%Point3f(0,0,1);

                                  /*
                                     double u =X- (int)X,v = Y - (int)Y, w= Z-(int)Z;
                                     if(u<0)	u = u+1;
                                     if(v<0)	v = v+1;
                                     if(w<0)	w = w+1;
                                     */
                                  double u =X- (int)X,v = Y - (int)Y;
                                  if(u<0)	u = u+1;
                                  if(v<0)	v = v+1;

                                  //if(( (X>0 && X<1) && (Y>0 && Y<1) ) )   {                                  u = X; v = Y;
                                  u = u*projectionImageWidth,v=v*projectionImageHeight;
                                  int pixmapIndex = abs((int)v * projectionImageWidth + (int)u) * 3;

                                  //printf("psi: %f , theta: %f , u: %f , v: %f \n",psi, theta, u , v);
                                  //cout<<(int)( (Y * projectionImageWidth + X) * 3 )<<endl; 
                                  //cout<< (float)pixmap[pixmapIndex]<<endl;
                                  finalColor.red = (float)(pixmap[pixmapIndex])/maxcolor;
                                  finalColor.green =(float)(pixmap[pixmapIndex + 1])/maxcolor;
                                  finalColor.blue = (float)(pixmap[pixmapIndex + 2])/maxcolor;


                                  finalColor = finalColor*(((Sphere*)allObjects[winIndex])->phongShader(myray,PL));
                                }

				if(mywinIndex!=-1 && softShadowFlag==0)
					finalColor = finalColor+  Color(0,0,0);

				if(cDirect==0)
					finalColor = finalColor*shadowColor;
                                				
				/*



				//print(interSectionPoint);cout<<endl;
				double psi = acos(Z);
				double theta = acos( Y/(sqrt((1-(Z*Z))) )  );
				if(abs( Y/( sqrt((1-(Z*Z)) )) > 1 )) 
					theta = asin(X/( sqrt((1-(Z*Z))) )  );
				if( abs(X/( sqrt((1-(Z*Z))) )) > 1 )
					cout<<"we are doomed!\n";


				double PI = 3.14;
                                double v = psi/PI, u = theta/(2*PI);

				//printf("psi: %f , theta: %f , u: %f , v: %f \n",psi, theta, u , v);
				if(X<0)	u=1-u;
                                X = u*projectionImageWidth,Y=v*projectionImageHeight;
                                int II = floor(X), JJ=floor(Y);
                                double s= X - floor(X), t = Y - floor(Y);

                                int pixmapIndex = abs(JJ * projectionImageWidth + II) * 3;
                                int pixmapIndexI1 = abs(JJ * projectionImageWidth + (II+1)) * 3;
                                int pixmapIndexJ1 = abs((JJ+1) * projectionImageWidth + II) * 3;
                                int pixmapIndexI1J1 = abs((JJ+1) * projectionImageWidth + (II+1)) * 3;
                                Color CIJ= Color(pixmap[pixmapIndex],pixmap[pixmapIndex+1], pixmap[pixmapIndex+2]);
                                Color CI1J= Color(pixmap[pixmapIndexI1],pixmap[pixmapIndexI1+1], pixmap[pixmapIndexI1+2]);
                                Color CIJ1= Color(pixmap[pixmapIndexJ1],pixmap[pixmapIndexJ1+1], pixmap[pixmapIndexJ1+2]);
                                Color CI1J1= Color(pixmap[pixmapIndexI1J1],pixmap[pixmapIndexI1J1+1], pixmap[pixmapIndexI1J1+2]);

                                finalColor = CIJ*(1-s)*(1-t) +CI1J*(s)*(1-t) +CIJ1*(1-s)*(t) +CI1J1*(s)*(t);
                                finalColor = finalColor/255.0;

				//printf("red: %f , greeb: %f , blue: %f \n",finalColor.red, finalColor.green,finalColor.blue );
				*/




				//double X = u*Xmax,Y=v*Ymax, u1 = X-floor(X), v1 = Y-floor(Y);
				//double X = u*projectionImageWidth,Y=v*projectionImageHeight, u1 = X-floor(X), v1 = Y-floor(Y);
				//int X = (int)u*projectionImageWidth,Y=(int)v*projectionImageHeight, u1 = X-floor(X), v1 = Y-floor(Y);





				//printf("psi: %f , theta: %f , u: %f , v: %f \n",psi, theta, u , v);
				//cout<<(int)( (Y * projectionImageWidth + X) * 3 )<<endl; 
				//cout<< (float)pixmap[(int)( (Y * projectionImageWidth + X) * 3 ) +1]<<endl;
				//finalColor.red = (float)(pixmap[(int)( (Y * projectionImageWidth + X) * 3 ) ])/maxcolor;
				//finalColor.green =(float)( pixmap[(int)((Y * projectionImageWidth + X) * 3 ) +1 ])/maxcolor ;
				//finalColor.blue = (float)( pixmap[(int)((Y * projectionImageWidth + X) * 3 ) +2 ])/maxcolor ;



			}
			if(allObjects[winIndex]->objectName=="Plane")	
			{
				if(spotlightEnabled)
					finalColor = finalColor+ (((Plane*)allObjects[winIndex])->phongShader(myray,PL))*alpha;
				else
				{
					//finalColor = finalColor+ ((Plane*)allObjects[winIndex])->lambertShader(myray,PL);

					finalColor = finalColor+  ((Plane*)allObjects[winIndex])->getColor();
				}


				//finalColor = finalColor+  ((Plane*)allObjects[winIndex])->getColor();
				if(mywinIndex!=-1 && softShadowFlag==0)
					finalColor = finalColor+  Color(0,0,0);
				if(cDirect==0)
					finalColor =  finalColor*shadowColor;

				double X,Y,S0=10,S1=10;
                                S0=100;
                                S1=S0;
                                Point3f planeNorm =  ((Plane*)allObjects[winIndex])->normalVector;
				planeNorm.Normalize();
				Point3f crossVec = planeNorm^Point3f(0,0,1);
				if(crossVec.x == 0 && crossVec.y == 0 && crossVec.z == 0 )
				{

					X =( (interSectionPoint - ((Plane*)allObjects[winIndex])->origin)%Point3f(1,0,0))/S0; 
					Y =( (interSectionPoint - ((Plane*)allObjects[winIndex])->origin)%Point3f(0,1,0))/S1; 
				}
				crossVec = planeNorm^Point3f(0,1,0);
				if(crossVec.x == 0 && crossVec.y == 0 && crossVec.z == 0 )
				{

					X =( (interSectionPoint - ((Plane*)allObjects[winIndex])->origin)%Point3f(1,0,0))/S0; 
					Y =( (interSectionPoint - ((Plane*)allObjects[winIndex])->origin)%Point3f(0,0,1))/S1; 
				}
				crossVec = planeNorm^Point3f(1,0,0);
				if(crossVec.x == 0 && crossVec.y == 0 && crossVec.z == 0 )
				{

					X =( (interSectionPoint - ((Plane*)allObjects[winIndex])->origin)%Point3f(0,1,0))/S0; 
					Y =( (interSectionPoint - ((Plane*)allObjects[winIndex])->origin)%Point3f(0,0,1))/S1; 
				}
				double u =X- (int)X,v = Y - (int)Y;
                                if(u<0)	u = u+1;
				if(v<0)	v = v+1;

                                //if(( (X>0 && X<1) && (Y>0 && Y<1) ) )   {                                  u = X; v = Y;
                               u = u*bumpImageWidth,v=v*bumpImageHeight;
				int pixmapIndex = abs((int)v * bumpImageWidth + (int)u) * 3;
				Color tmpColor;


                                tmpColor.red = (float)(bumpMap[pixmapIndex])/bumpmaxcolor;
                                tmpColor.green =(float)(bumpMap[pixmapIndex + 1])/bumpmaxcolor;
				tmpColor.blue = (float)(bumpMap[pixmapIndex + 2])/bumpmaxcolor;

				finalColor = finalColor*tmpColor;
				
				crossVec = planeNorm^Point3f(0,1,0);
				if(crossVec.x == 0 && crossVec.y == 0 && crossVec.z == 0 )
				{
					//finalColor = finalColor* ((Plane*)allObjects[winIndex])->getColor();
				}
				crossVec = planeNorm^Point3f(1,0,0);
				if(crossVec.x == 0 && crossVec.y == 0 && crossVec.z == 0 )
				{

					//finalColor = finalColor*((Plane*)allObjects[winIndex])->getColor();
				}
				//finalColor = finalColor * tmpColor;
//printf("psi: %f , theta: %f , u: %f , v: %f \n",psi, theta, u , v);
				//cout<<(int)( (Y * projectionImageWidth + X) * 3 )<<endl; 
				//cout<< (float)pixmap[pixmapIndex]<<endl;
			
                                //}
                                								//printf("red: %f , greeb: %f , blue: %f \n",finalColor.red, finalColor.green,finalColor.blue );


				//printf("Final color in iteration %i : \n", lightNum);
				//finalColor.printColor();


			}
			
			if(allObjects[winIndex]->objectName=="genericObject")	
			{
			    //cout<<"I do exist\n";
			    int numPlanes = ((GenericObject*)(allObjects[winIndex]))->triangles.faces.size(), faceindex =-1;
			    vector<Point3f> tmpVector;
			    //cout<<"numplanes: "<<numPlanes<<endl;
			    Point3f Ph = interSectionPoint;
			    vector<FacePoint> vectPoints;

			    

			for(int i = 0; i < numPlanes; i++ )
			{
				ObjMeshFace triangleFace = ((GenericObject*)allObjects[winIndex])->triangles.faces[i];
				Point3f triNorm(triangleFace.vertices[0].normal); 
				triNorm.Normalize();
				Plane triPlane( triNorm,triangleFace.vertices[0].pos );



				//print(triPlane.origin);
				Point3f P0 = triangleFace.vertices[0].pos;
				Point3f P1 = triangleFace.vertices[1].pos;
				Point3f P2 = triangleFace.vertices[2].pos;

				//Point3f interPoint = triPlane.getIntersectionPoint(ray) , Ph = interPoint;

				Point3f A0 = areaOfTriangle(P1, P2 , Ph);
				Point3f A1 = areaOfTriangle(P2, P0 , Ph);
				Point3f A2 = areaOfTriangle(P0, P1 , Ph);
				Point3f A  = areaOfTriangle(P0, P1 , P2);

				//cout<<"wx "<<A0.x/A.x<<endl;
				//cout<<"wy "<<A0.y/A.y<<endl;
				//cout<<"wz "<<A0.z/A.z<<endl;
				double w0 = ( A0.x + A0.y + A0.z )/(A.x + A.y + A.z);
				double w1 = ( A1.x + A1.y + A1.z )/(A.x + A.y + A.z);
				double w2 = ( A2.x + A2.y + A2.z )/(A.x + A.y + A.z);

				//printf("three ws: %f, %f, %f \n",w0,w1,w2);

				// if in same plane
				//if( w0 + w1 + w2 >= 0.99  && w0+w1+w2<=1.01 )
				//{
					// if inside the triangle
					//cout<<"in the same plane!!\n";
					double sumArea = A1.Length() + A2.Length() + A0.Length();

					//if( (w0 >=0 && w0 <=1) && (w1 >=0 && w1 <=1) &&  (w2 >=0 && w2 <=1) )
					if(sumArea >= 0.97*A.Length() && sumArea <= 1.03*A.Length())
					{
					    //cout<<"inside the triangle!!\n";

						faceindex = i;
						//cout<<"face index of this point: ";print(interSectionPoint);cout<<endl;
						Point3f centroid((triangleFace.vertices[0].pos + triangleFace.vertices[1].pos + triangleFace.vertices[2].pos)/3.0);

						FacePoint myPoint;
						myPoint.point = centroid;
						myPoint.faceindex= faceindex;
						vectPoints.pb(myPoint);


						break;
					}
					
				//}
			}
			//cout<<endl;

			//if(vectPoints.size() == 1)	faceindex = vectPoints[0].faceindex;
			//if(vectPoints.size() == 2)
			//	faceindex =( (Pe-vectPoints[0].point).Length() > (Pe-vectPoints[1].point).Length()) ? vectPoints[1].faceindex : vectPoints[0].faceindex;

			    finalColor = ((GenericObject*)allObjects[winIndex])->getColor();
                                //if(( (X>0 && X<1) && (Y>0 && Y<1) ) )   {                                  u = X; v = Y;
			


                            /*
				double u = ((GenericObject*)allObjects[winIndex])->triangles.faces[faceindex].vertices[0].texcoord.x;
				double v = ((GenericObject*)allObjects[winIndex])->triangles.faces[faceindex].vertices[0].texcoord.y;

                               u = u*bumpImageWidth,v=v*bumpImageHeight;
				int pixmapIndex = abs((int)v * bumpImageWidth + (int)u) * 3;
				Color tmpColor;


                                tmpColor.red = (float)(bumpMap[pixmapIndex])/bumpmaxcolor;
                                tmpColor.green =(float)(bumpMap[pixmapIndex + 1])/bumpmaxcolor;
				tmpColor.blue = (float)(bumpMap[pixmapIndex + 2])/bumpmaxcolor;

				finalColor = tmpColor;

                            // *
                            if(faceindex ==0 )	finalColor = Color(1,1,1);
                            if(faceindex ==1 )	finalColor = Color(1,0,1);
                            if(faceindex ==2 )	finalColor = Color(1,1,0);
                            if(faceindex ==3 )	finalColor = Color(0,1,1);
                            */



                            for(int i = 0; i < numPlanes; i++ )
                            {
                              ObjMeshFace triangleFace = ((GenericObject*)allObjects[winIndex])->triangles.faces[i];
                              Point3f triNorm(triangleFace.vertices[0].normal); 
                              triNorm.Normalize();
                              Plane triPlane( triNorm,triangleFace.vertices[0].pos );
                              //print(triPlane.origin);
                              Point3f P0 = triangleFace.vertices[0].pos;
                              Point3f P1 = triangleFace.vertices[1].pos;
                              Point3f P2 = triangleFace.vertices[2].pos;

                              //Point3f interPoint = triPlane.getIntersectionPoint(ray) , Ph = interPoint;

                              Point3f A0 = areaOfTriangle(P1, P2 , Ph);
                              Point3f A1 = areaOfTriangle(P2, P0 , Ph);
                              Point3f A2 = areaOfTriangle(P0, P1 , Ph);
                              Point3f A  = areaOfTriangle(P0, P1 , P2);

                              //cout<<"wx "<<A0.x/A.x<<endl;
                              //cout<<"wy "<<A0.y/A.y<<endl;
                              //cout<<"wz "<<A0.z/A.z<<endl;
                              double w0 = ( A0.x + A0.y + A0.z )/(A.x + A.y + A.z);
                              double w1 = ( A1.x + A1.y + A1.z )/(A.x + A.y + A.z);
                              double w2 = ( A2.x + A2.y + A2.z )/(A.x + A.y + A.z);

                              //printf("three ws: %f, %f, %f \n",w0,w1,w2);

                              // if in same plane
                              //if( w0 + w1 + w2 >= 0.99  && w0+w1+w2<=1.01 )
                              //{
                              // if inside the triangle
                              //cout<<"in the same plane!!\n";
                              double sumArea = A1.Length() + A2.Length() + A0.Length();

                              //if( (w0 >=0 && w0 <=1) && (w1 >=0 && w1 <=1) &&  (w2 >=0 && w2 <=1) )
                              if(sumArea >= 0.97*A.Length() && sumArea <= 1.03*A.Length())
                              {
                                //cout<<"inside the triangle!!\n";

                                faceindex = i; 
                                double X,Y,S0=10,S1=10,Z;
                                  S0=500;
                                  S1=S0;


                                  interSectionPoint =triNorm;
                                  //interSectionPoint = interSectionPoint/((Sphere*)allObjects[winIndex])->radius;

                                  X = interSectionPoint%Point3f(1,0,0);
                                  Y = interSectionPoint%Point3f(0,1,0);
                                  Z = interSectionPoint%Point3f(0,0,1);

                                  /*
                                     double u =X- (int)X,v = Y - (int)Y, w= Z-(int)Z;
                                     if(u<0)	u = u+1;
                                     if(v<0)	v = v+1;
                                     if(w<0)	w = w+1;
                                     */
                                  double u =X- (int)X,v = Y - (int)Y;
                                  if(u<0)	u = u+1;
                                  if(v<0)	v = v+1;

                                  //if(( (X>0 && X<1) && (Y>0 && Y<1) ) )   {                                  u = X; v = Y;
                                  double u0 = triangleFace.vertices[0].texcoord.x, u1 = triangleFace.vertices[1].texcoord.x, u2 = triangleFace.vertices[2].texcoord.x ;
                                  double v0 = triangleFace.vertices[0].texcoord.y, v1 = triangleFace.vertices[1].texcoord.y, v2 = triangleFace.vertices[2].texcoord.y ;

                                  u = w0*u0 + w1*u1 + w2*u2;
                                  v = w0*v0 + w1*v1 + w2*v2;
                                  if(u<0)	u = u+1;
                                  if(v<0)	v = v+1;
                                  if(u>0)	u = u-1;
                                  if(v>0)	v = v-1;


                                  //if(u>1 || v>1|| u<0 || v<0)  
                                  //  printf("absurd value: u: %f, v: %f", u,v);




                                  u = u*projectionImageWidth,v=v*projectionImageHeight;
                                  int pixmapIndex = abs((int)v * projectionImageWidth + (int)u) * 3;

                                  //printf("psi: %f , theta: %f , u: %f , v: %f \n",psi, theta, u , v);
                                  //cout<<(int)( (Y * projectionImageWidth + X) * 3 )<<endl; 
                                  //cout<< (float)pixmap[pixmapIndex]<<endl;
                                  finalColor.red = (float)(pixmap[pixmapIndex])/maxcolor;
                                  finalColor.green =(float)(pixmap[pixmapIndex + 1])/maxcolor;
                                  finalColor.blue = (float)(pixmap[pixmapIndex + 2])/maxcolor;


                                //cout<<"face index of this point: ";print(interSectionPoint);cout<<endl;
                                break;
                              }

                              //}
                            }





			    finalColor = finalColor*((GenericObject*)allObjects[winIndex])->goochShader(myray, PL);
                            //finalColor = finalColor * shadowColor;
				

			
			}

			pixels[index].r=finalColor.red;
			pixels[index].g=finalColor.green;
			pixels[index].b=finalColor.blue;
		}
	}
	time_t newTime;
	time(&newTime);
	std::string number;std::stringstream strstream;strstream << newTime;strstream >> number;
	string fileName = "scene_" + number + ".bmp";cout<<fileName;
	savebmp(fileName.c_str(),Xmax,Ymax,dpi,pixels);
	////////////////////////////////////////////////////////////////////////
	
	return 0;
	
}
