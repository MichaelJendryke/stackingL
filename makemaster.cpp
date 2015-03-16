#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <dirent.h> 
#include <sys/stat.h>

using namespace std;

int inputfiles(string, string, int, int, int ,int );

int makemaster(char* m, char* sdir, char* image, char* md, int r0, int rl, int c0, int cl, int quick=0)
{
	system("rm -rf MASTER");
	mkdir("MASTER", 0777);
	chdir("MASTER");
	mkdir("Inputfiles", 0777);
	mkdir("Outdata", 0777);
	mkdir("Outinfo", 0777);
	chdir("Inputfiles");
	//inputfiles(m, m, r0, rl, c0, cl);
	
	chdir("../");


	char mlink[2000];
	snprintf(mlink, sizeof(mlink), "ln -i %s%s/%s.xml %s.xml", sdir,image,image,md);
	cout << "INFO: Link " << mlink << endl;
	system(mlink);
	memset(mlink, 0, 100);
	
	//use any *.cos file Be careful with polarimetric data.
	snprintf(mlink, sizeof(mlink), "ln -s %s%s/IMAGEDATA/*.cos %s.cos", sdir,image,md);
	cout << "INFO: Link " << mlink << endl;
	system(mlink);


	//Read,(crop) and oversample Masterimage
	try {
		if (quick == 0){
		system("doris Inputfiles/m_readfiles; ");
		system("doris Inputfiles/m_crop; ");
		system("doris Inputfiles/m_ovs; ");
		
		string mystring [1000];
		int i = 0;
		std::ifstream file("master.res");
		std::string line;
		while (std::getline(file, line)) {
			cout << line << endl;
			mystring[i] = line;
			i++;
		}
		file.close();
		cout << i << "\n";


		ofstream fout("master.res");

		if (!fout) {
			cout << "Cannot open output file.\n";
			return 1;
		}

		int j;
		for (j = 0; j < i; j++) {

			if (mystring[j] == "Multilookfactor_range_direction:     		1") {
				fout << "Multilookfactor_range_direction:   		1" << endl;
				fout << "Multilookfactor_azimuth_direction:   		1" << endl;
				fout << "Multilookfactor_range_direction:   		1" << endl;
				fout << "Multilookfactor_azimuth_direction:   		1" << endl;
				fout << "Multilookfactor_range_direction:   		1" << endl;
				fout << "Multilookfactor_azimuth_direction:   		1" << endl;
				fout << "Multilookfactor_range_direction:   		1" << endl;

			} else {
				fout << mystring[j] << endl;
			}
		}
		fout.close();
		
		} else if(quick == 1){
		// Only needed for FINDSBAS
			system("doris Inputfiles/m_readfiles; ");
		}
		
	} catch (...) {
		cout << "ERROR: something went wrong" << endl;
	}


	chdir("../");
	

	return 0;
}

