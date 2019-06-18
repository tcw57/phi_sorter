#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cmath>
#include "hexbin.h"

using namespace std;
const int nphibits = 12;
float int_to_float(int int_num){
	float floatval = (int_num/(pow(2,nphibits-1)-1))*M_PI;
	return floatval;
}

int main(){
	const int nphibins = 27; //number of phi bins
	const int nphisectors = 9; //number of phi sectors
	const int tpe = 224; //track per event
	ifstream in_tracks[2][nphisectors];
	int ntrks[nphibins];
	for (int i = 0; i < nphisectors; ++i) {
		string fname1, fname2; //"1" is for positive eta files, "2" is for negative eta files.
		ifstream file1, file2;
		string s;
		stringstream out;
		out << i;
		s = out.str();
		fname1 = "phi" + s + "_n.dat";
		fname2 = "phi" + s + "_p.dat";
		in_tracks[0][i].open(fname1.c_str());
		in_tracks[1][i].open(fname2.c_str());
	}
	for (int j = 0; j < nphibins; ++j){
		ntrks[j] = 0;
	}
//This part is for debugging purposes/////////
	//string line;
	//while (getline(in_tracks[1][7],line)){
	//	cout << line << endl;
	//}
//////////////////////////////////////////////

//initialize new input files
	ofstream in_tracks_final[nphibins];
	for (int i = 0; i < nphibins; ++i){
		string fname;
		string s;
		stringstream out;
		out << i;
		s = out.str();
		fname = "phi" + s + ".dat";
		in_tracks_final[i].open(fname.c_str());
	}

	for (int i = 0; i < nphisectors; ++i){
		string line1, line2;
		for (int j = 0; j < nphibins; ++j){
            //fill each phi file, from 0 to 26, one event at a time
			while (getline(in_tracks[0][i],line1) && getline(in_tracks[1][i],line2) && ntrks[j] < 339){
				if (line1 != "0x000000000000000000000000" && line2 != "0x000000000000000000000000"){
					in_tracks_final[j] << line1 << endl;
					in_tracks_final[j] << line2 << endl;
					ntrks[j] += 2;
				}
				else if (line1 != "0x000000000000000000000000" && line2 == "0x000000000000000000000000" && ntrks[j] < 339){
					in_tracks_final[j] << line1 << endl;
					ntrks[j] += 1;
				}
				else if (line1 == "0x000000000000000000000000" && line2 != "0x000000000000000000000000" && ntrks[j] < 339){
					in_tracks_final[j] << line2 << endl;
					ntrks[j] += 1;
				}
				else{
					for (int k = 0; k < (tpe - ntrks[j]); ++k){
						in_tracks_final[j] << "0x000000000000000000000000" << endl;
					}
                    			continue;
				}
			}
			in_tracks_final[j].close();
			in_tracks[0][i].close();
			in_tracks[1][i].close();
		}

	}

	return 0;
}
