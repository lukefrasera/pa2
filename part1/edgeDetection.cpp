// Luke Fraser
// PA2: Canny Edge Detector
#include "opencv2/opencv.hpp"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <fnmatch.h>


using namespace cv;
using namespace std;

int main( int argc, char ** argv ){
	
	// import images
	string imageDir = "./images/parta/";
	Mat * images;
	Mat edges;

	double thr1, thr2;
	int aper;

	thr1 = atof(argv[1]);
	thr2 = atof(argv[2]);
	aper = atoi(argv[3]);

	DIR * dir;
	struct dirent * ent;
	list<string> files;
	list<string>::iterator it;

	static const char * filter = "*.pgm";

	it = files.begin();

	if( argc != 4 )
		return -1;


	printf( "%s\n", argv[2]);
	if((dir = opendir(imageDir.c_str())) != NULL ){
		while((ent = readdir (dir)) != NULL){
			string temp;
			if(fnmatch(filter, ent->d_name, FNM_CASEFOLD) == 0){
				files.insert(it, (temp = ent->d_name));
				++it;
			}
		}
		closedir(dir);
	}

	images = new Mat[files.size()];

	int i;
	for( it = files.begin(), i = 0; it != files.end(); ++it, i++){
		images[i] = imread((imageDir+(*it)).c_str());
		printf("Image: %s Uploaded\n", (*it).c_str());
	}
	// create mats for edge detection

	// perform edge detection
	for( i = 0, it = files.begin(); i < files.size(); i++, ++it){
		edges = images[i].clone();

		GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);

		//void Canny(const Mat& image, Mat& edges, double threshold1, double threshold2, int apertureSize=3, bool L2gradient=false)¶
		Canny(edges, edges, thr1,thr2, aper, true);
		imwrite(("./images/edges"+(*it)).c_str(), edges);
	}

	// save out images
	return 0;
}