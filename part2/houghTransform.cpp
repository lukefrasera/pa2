// Luke Fraser
// PA2: Canny Edge Detector
#include "opencv2/opencv.hpp"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <fnmatch.h>
#include <cmath>


using namespace cv;
using namespace std;

bool houghTrans(const Mat &, Mat & );

int main( int argc, char ** argv ){
	
	// import images
	string imageDir = "./images/";
	Mat * images;
	Mat edges;
	Mat circles;

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
		circles = edges.clone();
		houghTrans(edges,circles);
		imwrite(("./images/result/edges"+(*it)).c_str(), edges);
	}


	// save out images
	return 0;
}


bool houghTrans(const Mat & edges, Mat & overlay){
	// quantize parameter face x0, y0, r
	int *** acc;
	int maxradius;
	int width, height, xcent, ycent;


	width = edges.size().width;
	height = edges.size().height;
	maxradius = max(width/2, height/2);

	// allocate
	acc = new int**[maxradius];

	for(int i = 0; i < maxradius; i++){
		acc[i] = new int*[height];
		for(int j = 0; j < height; j++){
			acc[i][j] = new int[width];
			for(int k = 0; k < width; k++){
				acc[i][j][k] = 0;
			}
		}
	}

	// for each edge point
	for(int i = 0; i < height; i++){
		for( int j = 0; j < width; j++){
		// for each Radius : r
			maxradius = min(height/2 - abs(height/2 - i) , width/2 - abs(width/2 - j));
			if( edges.at<uchar>(i,j) == 255){

				for( int k = 10; k < maxradius; k++){
					for( int l = 0; l < 360; l++){
					// for each theta: 0

						// calc y0 for circle
						xcent = round(j + k * cos(l));

						ycent = round(i + k * sin(l));

						// vote at location
						if( xcent >= 0 && xcent < width && ycent >= 0 && ycent < height )
							acc[k][ycent][xcent]++;
					}
				}

			}


		}
	}


	// find local maxima

	//remove sperious circles

	// create linked list of circle edges

	// draw cricles to image

	// print circle information for each circle
}