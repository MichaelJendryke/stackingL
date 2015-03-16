/* 
 * File:   main.cpp
 * Author: Michael Jendryke
 *
 * Created on March 2, 2012, 5:34 PM
 */
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h> 
#include <sys/stat.h>
#include <sstream>
#include <unistd.h>
#include <string>
#include <algorithm>
#include "tools.h"

using namespace std;
// external functions
int inputfiles(string, string, char*, int, int, int, int);
int makemaster(char*, char*, char*, char*, int, int, int, int, int quick = 0);
void makepair(int first, int last, string master, string slave, string imagestackfiles[], char* sourcedir, char* sensor, int r0, int rl, int c0, int cl);

void information(void) {
    cout << " _________________________________________________________________ " << endl;
    cout << "|                                                                 |" << endl;
    cout << "|                             TSI                                 |" << endl;
    cout << "|         TerraSAR-X Spotlight Interferogram Processor            |" << endl;
    cout << "|                                                                 |" << endl;
    cout << "|                  Matlab and DORIS are required                  |" << endl;
    cout << "|                                                                 |" << endl;
    cout << "|                Michael Jendryke and Jiang Houjun                |" << endl;
    cout << "|                contact: michaeljendryke@gmail.com               |" << endl;
    cout << "|                                                                 |" << endl;
    cout << "|             Version 1.0a                    2011-10-24          |" << endl;
    cout << "|_________________________________________________________________|" << endl;
    cout << "" << endl;
    cout << "This program creates Interferograms from TSX SPOTLIGHT images." << endl;
    cout << "" << endl;
    cout << "The software has been tested on WIN7 64bit with Cygwin." << endl;
    cout << "Software dependencies:  DORIS(with gdal, python and lxml)" << endl;
    cout << "			Matlab2011a or Matlab Runtime Compiler 2011a" << endl;
    cout << "			SPOTLIGHT_2.exe (precompiled Matlab scripts)" << endl;
    cout << "			SPOTLIGHT_3.exe (precompiled Matlab scripts)" << endl;
    cout << "" << endl;
    cout << "To install lxml (with libxml2 and libxstl) in PYTHON, download the " << endl;
    cout << "easy_install script from python and install PIP" << endl;
    cout << "$ python" << endl;
    cout << "  >>ez_setup.py" << endl;
    cout << "  >> easy_install pip" << endl;
    cout << "  then" << endl;
    cout << "  >> pip install lxml" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "How to use the software:" << endl;
    cout << "" << endl;
    cout << "Create \"imagestack.txt\" (ANSI) in your working directory " << endl;
    cout << "This file is a list of all images that should be considered" << endl;
    cout << "THE FIRST LINE IS THE MASTER IMAGE!" << endl;
    cout << "(Hint: use \'ls /SOURCEDIR/ > imagestack.txt)" << endl;
    cout << "List all original image names like:" << endl;
    cout << "." << endl;
    cout << "TSX1_SAR__SSC______HS_S_SRA_20100716T053350_20100716T053351" << endl;
    cout << "TSX1_SAR__SSC______HS_S_SRA_20101103T053354_20101103T053355" << endl;
    cout << "TSX1_SAR__SSC______HS_S_SRA_20100920T053354_20100920T053354" << endl;
    cout << "TSX1_SAR__SSC______HS_S_SRA_20110130T053349_20110130T053350" << endl;
    cout << "." << endl;
    cout << "" << endl;
    cout << "START THE PROCESS WITH:" << endl << endl;
    cout << "  " << "[PROGRAM] [FIRST STEP] [LAST STEP] [/SOURCEDIR/]" << endl << endl;
    cout << "" << endl;
    cout << "(SOURCEDIR is the folder where your Images are, use same drive!)" << endl;
    cout << "" << endl;
    cout << "List of steps" << endl;
    cout << "/*  0 */  -h," << endl;
        cout << "/*  1 */  Inputfiles/m_readfiles, //includes porbits if avaliable" << endl;
        cout << "/*  2 */  Inputfiles/s_readfiles, //includes porbits if avaliable" << endl;
        cout << "/*  3 */  Inputfiles/coarseorb," << endl;
        cout << "/*  4 */  Inputfiles/m_crop;," << endl;
        cout << "/*    */ // Inputfiles/m_simamp;," << endl;
        cout << "/*    */ // Inputfiles/m_timing;," << endl;
        cout << "/*  5 */  Inputfiles/m_ovs;," << endl;
        cout << "/*  6 */  Inputfiles/s_crop;," << endl;
        cout << "/*  7 */  Inputfiles/s_ovs;," << endl;
        cout << "/*  8 */  Inputfiles/coarsecorr;," << endl;
        cout << "/*    */ // Inputfiles/m_filtazi;,	" << endl;
        cout << "/*    */ // Inputfiles/s_filtazi;," << endl;
        cout << "/*  9 */  Inputfiles/fine;," << endl;
        cout << "/*    */ // Inputfiles/reltiming;," << endl;
        cout << "/*    */ // Inputfiles/demassist;," << endl;
        cout << "/* 10 */  Inputfiles/coregpm;," << endl;
        cout << "/*    */ //SPOTLIGHT_2.exe,	" << endl;		
        cout << "/*    */ //SPOTLIGHT_2,  " << endl;                        
        cout << "/*    */ //matlab -nosplash -nodesktop -r SPOTLIGHT_2, " << endl;   
        cout << "/* 11 */  Inputfiles/resample;," << endl;
        cout << "/*    */ //SPOTLIGHT_3.exe,	" << endl;		
        cout << "/*    */ //SPOTLIGHT_3,  " << endl;                          
        cout << "/*    */ //matlab -nosplash -nodesktop -r SPOTLIGHT_3,  " << endl; 
        cout << "/*    */ // Inputfiles/filtrange;,	" << endl;
        cout << "/* 12 */  Inputfiles/interfero;," << endl;
        cout << "/* 13 */  Inputfiles/comprefpha;," << endl;
        cout << "/* 14 */  Inputfiles/subtrrefpha;," << endl;
        cout << "/* 15 */  Inputfiles/comprefdem;," << endl;
        cout << "/* 16 */  Inputfiles/subtrrefdem;," << endl;
        cout << "/* 17 */  Inputfiles/coherence;," << endl;
        cout << "/* 18 */  Inputfiles/filtphase;," << endl;
        cout << "/* 19 */  Inputfiles/unwrap;," << endl;
        cout << "/* 20 */  Inputfiles/dinsar;," << endl;
        cout << "/* 21 */  Inputfiles/slant2h;" << endl;
        cout << "/* 22 */  Inputfiles/geocode;" << endl;
    cout << "" << endl << endl;
    cout << "EXAMPLE: [PROGRAM] 0 8 /SOURCE/Shanghai ERS1CEOS SM firstrow lastrow firstcolumn lastcolumn" << endl << endl;
    cout << "Process all images in imagestack.txt from 0 to 8 in ERS1CEOS in SingleMaster" << endl;
    cout << "" << endl;
    cout << "Supported Fileformat: ERS1CEOS, ERS1E1, ERS2CEOS, TSX" << endl;
    cout << "" << endl;
    cout << "No support for seperate steps so far! Only for last step." << endl << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "Enjoy!" << endl;
    cout << "Compiled on: " __DATE__ << " at: " << __TIME__ << endl;
}

void usage(void) {
    cout << "" << endl;
    cout << "[PROGRAM] [FIRST STEP] [LAST STEP] [/SOURCEDIR/] [ERS1CEOS | ERS2CEOS | ERS1E1| TSX] [SM | SBAS]" << endl;
    cout << "  OR" << endl;
    cout << "[PROGRAM] [-h] to get HELP" << endl;
}

std::string get_working_path() {
    char temp[1024];
    return ( getcwd(temp, 1024) ? std::string(temp) : std::string(""));
}

float stof(string str) {
    stringstream sstr;
    //string str = "1234.23";
    float f;
    sstr << str;
    sstr >> f;
    return f;
}

float stod(string str) {
    stringstream sstr;
    //string str = "1234.23";
    double d;
    sstr << str;
    sstr >> d;
    return d;
}

int ctoi(char* data, int size) {
    std::string myString(data, size);
    stringstream sstr;
    //string str = "1234.23";
    int i;
    sstr << myString;
    sstr >> i;
    return i;
}

double ctod(char* data) {
    int size = sizeof (data);
    std::string myString(data, size);
    stringstream sstr;
    //string str = "1234.23";
    double d;
    sstr << myString;
    sstr >> d;
    return d;
}

double abs(double z) {
    if (z >= 0) {
        return z;
    }
    return -z;
}

float abs(float z) {
    if (z >= 0) {
        return z;
    }
    return -z;
}



//void sbas(double t[], double p[], int l, int d[])
//{
//
//	cout << "SBAS result for: " << l << endl;
//
//
//	//cout << "Baseline between: " << d[0] << " and " << d[0] << " = \t" << t[0] << "\t" << p[0] << endl;
//	for (int i = 0; i < l + 1; i++) {
//
//		cout << "Baseline between: " << d[0] << " and " << d[i] << " = \t" << t[i] << "\t" << p[i] << endl;
//
//	}
//
//	cout << "This is a list of the image dates and temporal and perpendicular baselines" << endl;
//	cout << "In order to process SBAS please enter the max time and the max m" << endl;
//	int days = 100;
//	int meter = 100;
//	cout << "max days: ";
//	cin >> days;
//	cout << "max meter: ";
//	cin >> meter;
//
//	//calculate baselines
//	for (int x = 0; x < l + 1; x++) {
//		if (abs(t[x + 1]) < days && p[x + 1] < meter)
//			cout << d[0] << d[x] << t[x + 1] << p[x + 1] << endl;
//	}
//
//	for (int x = 0; x < l + 1; x++) {
//		for (int y = 1; y < l; y++) {
//			if (abs(abs(t[x]) - abs(t[y])) < days)
//
//				//abs(t[x + 1]) < days && p[x + 1] < meter)
//				cout << d[x] << "_" << d[y] << "\t" << abs(abs(t[x]) - abs(t[y])) << endl;
//
//			//cout << d[0] << d[x] << t[x + 1] << p[x + 1] << endl;
//		}
//	}
//
//}

int baselines(string master, string slave, float d = 100000, float m = 100000) {

    std::ifstream file("master_slave.res");
    std::string line;
    std::string btempstr;
    std::string bperpstr;
    bool two = false;

    while (std::getline(file, line)) {
        btempstr = line;
        bperpstr = line;
        double bt;
        float bp;
        line.erase(line.begin() + 7, line.end());

        if (line == "  Btemp") {
            btempstr.erase(btempstr.begin(), btempstr.begin() + 25);
            btempstr.erase(btempstr.begin() + 9, btempstr.end());
            //btempstr.erase(remove(btempstr.begin(), btempstr.end(), ' '), btempstr.end());
            bt = stod(btempstr);
        } else if (line == "  Bperp") {
            bperpstr.erase(bperpstr.begin(), bperpstr.begin() + 25);
            bperpstr.erase(bperpstr.begin() + 9, bperpstr.end());
            //bperpstr.erase(remove(bperpstr.begin(), bperpstr.end(), ' '), bperpstr.end());
            bp = stof(bperpstr);
            //cout << "btempstr: " << bt << "  bperpstr: " << bp << endl;
            two = true;

        } else {
            //cout << "Nothing found" << endl;
        }
        if (two == true && bt != 0 && bt < d && bp != 0 && bp < m) {
            ofstream fout("../Baselines.csv", ios::out | ios::app);
            if (!fout) {
                cout << "Cannot open output file.\n";
                return 1;
            }
            fout << master << "," << slave << "," << bt << "," << bp << endl;
            fout.close();

            two = false;
            bt = 0;
            bp = 0;
        }
    }
    file.close();


}

int main(int argc, char *argv[]) {
    // <editor-fold defaultstate="collapsed" desc="Declare variables">
    // Define string for system command ss
    stringstream ss(stringstream::in | stringstream::out);
    // declare variables
    int firststep, laststep, r0, rl, c0, cl;
    char* sourcedir;
    char* sensor;
    char* method;
    char* workingdir;
    string imagestackfiles[255];
    int numberofimages = 0;
    workingdir = string2charstar(get_working_path());
    int combinations;
    int combinationscount = 1;
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Check INPUT arguments">
    switch (argc) {
        case 0:
        case 1: // Call w/o arguments
            usage();
            exit(1);
        case 2:
            if (strcmp(argv[1], "-h") == 0) {
                information();
                exit(1);
            } else {
                cout << "Unknown input!" << endl;
                exit(1);
            }
        case 3:
        case 4:
            cerr << "Not enough input arguments" << endl;
            usage();
            exit(1);
        case 5:
        case 6:
            firststep = atoi(argv[1]);
            laststep = atoi(argv[2]);
            sourcedir = argv[3]; // check if directory
            sensor = argv[4];
            method = argv[5]; // check if one of the three methods
            r0 = 0;
            rl = 0;
            c0 = 0;
            cl = 0;
            break;
        case 7:
        case 8:
        case 9:
            cerr << "Not enough input arguments" << endl;
            usage();
            exit(1);
        case 10:
            firststep = atoi(argv[1]);
            laststep = atoi(argv[2]);
            sourcedir = argv[3]; // check if directory
            sensor = argv[4];
            method = argv[5]; // check if one of the three methods
            r0 = atoi(argv[6]);
            rl = atoi(argv[7]);
            c0 = atoi(argv[8]);
            cl = atoi(argv[9]);
            break;
        default:
            cout << "ERROR! Check input arguments" << endl;
            exit(1);

    }// </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Overview and read imagestack.txt file">
    cout << endl << "=====================================================" << endl << endl;
    cout << "OVERVIEW" << endl;
    cout << "Processing from \tStep " << firststep << " to " << "Step " << laststep << endl;
    cout << "Working directory is:\t" << workingdir << endl;
    cout << "Source directory is:\t" << sourcedir << endl;
    cout << "Processing extent:\tfirst line (azimuth):\t" << r0 << endl;
    cout << "\t\t\tlast line (azimuth):\t" << rl << endl;
    cout << "\t\t\tfirst pixel (range):\t" << c0 << endl;
    cout << "\t\t\tlast pixel(range):\t" << cl << endl;
    cout << "Sensor:\t\t\t" << sensor << endl;
    cout << "Method:\t\t\t" << method << endl << endl;
    cout << "Checking image directories" << endl;
    //read file
    std::ifstream imagestack("imagestack.txt");
    if (!imagestack.is_open()) {
        perror("ERROR while opening file \"imagestack.txt\"");
        exit(1);
    } else if (imagestack.good()) {
        std::string imagefile;
        int i = 0;
        // Imagestack file for TSX
        if (strcmp(sensor, "TSX") == 0) {
            while (std::getline(imagestack, imagefile)) {
                if (imagefile.length() >= 59) {
                    imagefile.erase(59, 1); // only TSX
                    imagestackfiles[i] = imagefile;
                    //check if folder exists
                    struct stat st;
                    string check;
                    std::string str(sourcedir);
                    check = str + imagefile;
                    const char * c = check.c_str();
                    if (stat(c, &st) == 0) {
                        cout << "Image NO: " << i + 1 << "\t" << check << " OK" << endl;
                        i++;
                    } else {
                        cout << "ERROR: Line " << i + 1 << "\t" << check << " FAIL" << endl;
                    }
                } else if (imagefile.length() < 59) {
                    cout << "ERROR bad filename found" << endl;
                    exit(1);
                }

            }
            // Imagestack file for ERS1CEOS and ERS2CEOS   
        } else if (strcmp(sensor, "TSX_SP") == 0) {
            while (std::getline(imagestack, imagefile)) {
                if (imagefile.length() >= 59) {
                    imagefile.erase(59, 1); // only TSX
                    imagestackfiles[i] = imagefile;
                    //check if folder exists
                    struct stat st;
                    string check;
                    std::string str(sourcedir);
                    check = str + imagefile;
                    const char * c = check.c_str();
                    if (stat(c, &st) == 0) {
                        cout << "Image NO: " << i + 1 << "\t" << check << " OK" << endl;
                        i++;
                    } else {
                        cout << "ERROR: Line " << i + 1 << "\t" << check << " FAIL" << endl;
                    }
                } else if (imagefile.length() < 59) {
                    cout << "ERROR bad filename found" << endl;
                    exit(1);
                }

            }
            // Imagestack file for ERS1CEOS and ERS2CEOS   
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0) {
            while (std::getline(imagestack, imagefile)) {
                if (imagefile.length() >= 14) {
                    imagefile.erase(14, 1); // only for ERS1CEOS and ERS2CEOS
                    imagestackfiles[i] = imagefile;
                    //check if folder exists
                    struct stat st;
                    string check;
                    std::string str(sourcedir);
                    check = str + imagefile;
                    const char * c = check.c_str();
                    if (stat(c, &st) == 0) {
                        cout << "Image NO: " << i + 1 << "\t" << check << " OK" << endl;
                        i++;
                    } else {
                        cout << "ERROR: Line " << i + 1 << "\t" << check << " FAIL" << endl;
                    }
                } else if (imagefile.length() < 14) {
                    cout << "ERROR bad filename found" << endl;
                    exit(1);

                }
            }
        } else if (strcmp(sensor, "ERS1E1") == 0 || strcmp(sensor, "ERS2E2") == 0 || strcmp(sensor, "ASARIMS") == 0) {
            while (std::getline(imagestack, imagefile)) {
                if (imagefile.length() >= 62) {
                    imagefile.erase(62, 1); // only for ERS1CEOS and ERS2CEOS
                    imagestackfiles[i] = imagefile;
                    //check if folder exists
                    struct stat st;
                    string check;
                    std::string str(sourcedir);
                    check = str + imagefile;
                    const char * c = check.c_str();
                    if (stat(c, &st) == 0) {
                        cout << "Image NO: " << i + 1 << "\t" << check << " OK" << endl;
                        i++;
                    } else {
                        cout << "ERROR: Line " << i + 1 << "\t" << check << " FAIL" << endl;
                    }
                } else if (strcmp(sensor, "ASARWSS") == 0) {

                } else if (imagefile.length() < 14) {
                    cout << "ERROR bad filename found" << endl;
                    exit(1);

                }
            }
        } else {
            cout << "sensor: " << sensor << " unknown" << endl;
            exit(1);
        }

        imagestack.close();
        numberofimages = i;
        cout << "Total number of images: " << numberofimages << endl;
    }
    cout << endl << "=====================================================" << endl << endl;
    
    // </editor-fold>

    if (strcmp(method, "SBAS") == 0) {
        // <editor-fold defaultstate="collapsed" desc=" SBAS ">
        cout << "SBAS" << endl;
        mkdir("SBAS", 0777);

        string md[255];
        string sd[255];
        //read file
        std::ifstream combCSV("combinations.csv");
        if (!combCSV.is_open()) {
            perror("ERROR while opening file \"combinations.csv\"");
            exit(1);
        } else if (combCSV.good()) {
            std::string cc;
            int i = 0;
            while (std::getline(combCSV, cc)) {
                string line = cc;
                cc.erase(cc.begin() + 8, cc.end());
                md[i] = cc;
                cc = line;
                cc.erase(cc.begin(), cc.begin() + 9);
                cc.erase(cc.begin() + 8, cc.end());
                sd[i] = cc;
                cout << md[i] << " and " << sd[i] << endl;
                i++;
            }
            combinations = i;
            cout << combinations << " combinations found" << endl;
            combCSV.close();
        }

        chdir("SBAS");
        for (int i = 0; i < combinations; i++) {
            string master = md[i];
            string slave = sd[i];
            string dir = master + "_" + slave;
            mkdir(dir.c_str(), 0777);
            chdir(dir.c_str());
            makepair(firststep, laststep, master, slave, imagestackfiles, sourcedir, sensor, r0, rl, c0, cl);
            baselines(master, slave);
            chdir("../");
        }
        chdir("../");
        exit(1);

        // </editor-fold>

    } else if (strcmp(method, "FINDSBAS") == 0) {
        // <editor-fold defaultstate="collapsed" desc=" FINDSBAS ">
        cout << "FINDSBAS" << endl;
        mkdir("FINDSBAS", 0777);
        chdir("FINDSBAS");
        float days = 10000;
        float meter = 10000;
        cout << "Maximum number of days [days]: ";
        cin >> days;
        cout << "Maximum meter of Bperp [meter]: ";
        cin >> meter;
        combinations = numberofimages * 0.5 * (numberofimages - 1);
        for (int i = 0; i <= numberofimages - 1; i++) {
            string master = imagestackfiles[i];
            master.erase(master.begin(), master.begin() + 28);
            master.erase(master.begin() + 8, master.end());

            for (int j = i + 1; j <= numberofimages - 1; j++) {
                string slave = imagestackfiles[j];
                slave.erase(slave.begin(), slave.begin() + 28);
                slave.erase(slave.begin() + 8, slave.end());
                cout << combinationscount << " of " << combinations << " Combinations. This is: " << master << "_" << slave << endl;
                combinationscount++;

                string dir = master + "_" + slave;
                mkdir(dir.c_str(), 0777);
                chdir(dir.c_str());
                makepair(1, 3, master, slave, imagestackfiles, sourcedir, sensor, r0, rl, c0, cl);
                baselines(master, slave, days, meter);
                chdir("../");
            }
        }
        chdir("../");
        exit(1);
        // </editor-fold>

    } else if (strcmp(method, "SM") == 0) {
        // <editor-fold defaultstate="collapsed" desc=" SM ">
        cout << "Will do Single Master (SM) frist image in imagestack.txt" << endl;
        mkdir("SM", 0777);
        chdir("SM");
        if (strcmp(sensor, "TSX") == 0  || strcmp(sensor, "TSX_SP") == 0) {
            for (int i = 1; i <= numberofimages - 1; i++) {
                string master = imagestackfiles[0];
                master.erase(master.begin(), master.begin() + 28);
                master.erase(master.begin() + 8, master.end());

                string slave = imagestackfiles[i];
                slave.erase(slave.begin(), slave.begin() + 28);
                slave.erase(slave.begin() + 8, slave.end());

                string dir = master + "_" + slave;
                mkdir(dir.c_str(), 0777);
                chdir(dir.c_str());
                makepair(firststep, laststep, master, slave, imagestackfiles, sourcedir, sensor, r0, rl, c0, cl);
                baselines(master, slave);
                chdir("../");
            }
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0) { //by CATALOG.IEF file
            //Master
            string metafile = sourcedir + imagestackfiles[0] + "/SCENE1/VDF_DAT.001";
            string master, slave, dir;

            std::ifstream imagestack(metafile.c_str());
            if (!imagestack.is_open()) {
                perror("ERROR while opening file %s");
                cout << metafile.c_str() << "\n";
                exit(1);
            } else if (imagestack.good()) {
                std::string imagefile;
                int i = 0;
                while (std::getline(imagestack, imagefile)) {
                    //cout << imagefile << endl;
                    string find("DATE");
                    size_t found;
                    found = imagefile.find(find);
                    imagefile.erase(imagefile.begin(), imagefile.begin() + int(found) + 5);
                    imagefile.erase(imagefile.begin() + 11, imagefile.end());
                    string com = "dateconv " + imagefile + " %d-%b-%Y %Y%m%d";
                    master = exec(string2charstar(com));
                    master.erase(master.begin() + 8, master.end());
                    cout << "Master date: " << master << endl;
                }
            }

            for (int i = 1; i <= numberofimages - 1; i++) {

                //Slave
                metafile = sourcedir + imagestackfiles[i] + "/SCENE1/VDF_DAT.001";
                //cout << metafile << endl;
                std::ifstream imagestack(metafile.c_str());
                if (!imagestack.is_open()) {
                    perror("ERROR while opening file \"/SCENE1/VDF_DAT.001\"");
                    exit(1);
                } else if (imagestack.good()) {
                    std::string imagefile;
                    int i = 0;
                    while (std::getline(imagestack, imagefile)) {
                        //cout << imagefile << endl;
                        string find("DATE");
                        size_t found;
                        found = imagefile.find(find);
                        imagefile.erase(imagefile.begin(), imagefile.begin() + int(found) + 5);
                        imagefile.erase(imagefile.begin() + 11, imagefile.end());
                        string com = "dateconv " + imagefile + " %d-%b-%Y %Y%m%d";
                        slave = exec(string2charstar(com));
                        slave.erase(slave.begin() + 8, slave.end());
                        cout << "Slave date: " << slave << endl;
                    }

                }

                dir = master + "_" + slave;

                mkdir(dir.c_str(), 0777);
                chdir(dir.c_str());
                makepair(firststep, laststep, master, slave, imagestackfiles, sourcedir, sensor, r0, rl, c0, cl);
                //baselines(master, slave);
                chdir("../");
            }

            exit(1);
        } else if (strcmp(sensor, "ERS1E1") == 0 || strcmp(sensor, "ERS2E2") == 0 || strcmp(sensor, "ASARIMS") == 0) {
            for (int i = 1; i <= numberofimages - 1; i++) {
                string master = imagestackfiles[0];
                master.erase(master.begin(), master.begin() + 14);
                master.erase(master.begin() + 8, master.end());

                string slave = imagestackfiles[i];
                slave.erase(slave.begin(), slave.begin() + 14);
                slave.erase(slave.begin() + 8, slave.end());

                float masterdate = stod(master);
                float slavedate = stod(slave);

                string dir = master + "_" + slave;
                mkdir(dir.c_str(), 0777);
                chdir(dir.c_str());
                makepair(firststep, laststep, master, slave, imagestackfiles, sourcedir, sensor, r0, rl, c0, cl);
                //baselines(master, slave);
                chdir("../");
            }
        } else if (strcmp(sensor, "ERS1CEOStest") == 0) {
            for (int i = 1; i <= numberofimages - 1; i++) {
                string master = imagestackfiles[0];
                master.erase(master.begin(), master.begin() + 9);

                string slave = imagestackfiles[i];
                slave.erase(slave.begin(), slave.begin() + 9);

                float masterdate = stod(master);
                float slavedate = stod(slave);

                string dir = master + "_" + slave;
                mkdir(dir.c_str(), 0777);
                chdir(dir.c_str());
                //makepair(firststep, laststep, master, slave, imagestackfiles, sourcedir, r0, rl, c0, cl);
                //baselines(master, slave);
                chdir("../");
            }
        }

        // </editor-fold>
    } else {
        cout << "ERROR: Method not recognized!" << endl;
    }
    return 0;
}
