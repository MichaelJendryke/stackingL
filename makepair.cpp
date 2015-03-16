#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <string>
#include "tools.h"



using namespace std;
int inputfiles(string m, string s, char* sensor, int r0, int rl, int c0, int cl);

void stepheadline(string c, int n) {
    cout << "                                                        " << endl;
    cout << "                                                        " << endl;
    cout << "                                                        " << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "                                                        " << endl;
    cout << "\tDoing step: " << n << " \t" << c << endl;
    cout << "                                                        " << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

}

void endofprocessing() {
    cout << "                                                        " << endl;
    cout << "                                                        " << endl;
    cout << "                                                        " << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "                                                        " << endl;
    cout << "\tDONE, thanks for using this software    " << endl;
    cout << "                                                        " << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}

int fixmaster() {

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
        return 0;
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
}

bool fexists(const char *filename) {
    ifstream ifile(filename);
    return ifile;
}

void makepair(int first, int last, string m, string s, string is[], char* sd, char* satsensor, int r0, int rl, int c0, int cl) {
    cout << "Processing image pair " << "from step " << first << " to " << last << endl;
        cout << "HERE!!";
    int i = 0;
    int mcurrent;
    int scurrent;

    if (strcmp(satsensor, "TSX") == 0) {
        while (is[i].size() != 0) {

            if (is[i].compare(28, 7, m, 0, 7) == 0) {
                cout << "Master: " << is[i] << " " << m << endl;
                mcurrent = i;

            }

            if (is[i].compare(28, 7, s, 0, 7) == 0) {
                cout << "Slave: " << is[i] << " " << s << endl;
                scurrent = i;
            }
            i++;

        }
    } else if (strcmp(satsensor, "ERS1CEOS") == 0 || strcmp(satsensor, "ERS2CEOS") == 0) {
        while (is[i].size() != 0) {
            string metafile, current;
            metafile = sd + is[i] + "/SCENE1/VDF_DAT.001";

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
                    current = exec(string2charstar(com));
                    current.erase(current.begin() + 8, current.end());

                }

            }

            if (strcmp(m.c_str(), current.c_str()) == 0) {
                cout << "Master: " << is[i] << " " << m << endl;
                mcurrent = i;

            }

            if (strcmp(s.c_str(), current.c_str()) == 0) {
                cout << "Slave: " << is[i] << " " << s << endl;
                scurrent = i;
            }

            i++;
        }
    } else if (strcmp(satsensor, "ERS1E1") == 0 || strcmp(satsensor, "ERS2E2") == 0 || strcmp(satsensor, "ASARIMS") == 0) {
               while (is[i].size() != 0) {
            if (is[i].compare(14, 7, m, 0, 7) == 0) {
                cout << "Master: " << is[i] << " " << m << endl;
                mcurrent = i;

            }

            if (is[i].compare(14, 7, s, 0, 7) == 0) {
                cout << "Slave: " << is[i] << " " << s << endl;
                scurrent = i;
            }

            i++;
        }
    }




    try {
        ofstream imag("images.txt");
        if (!imag) {
            cout << "Cannot create protocol file images.txt.\n";

        } else {
            imag << is[mcurrent] << endl;
            imag << is[scurrent] << endl;
        }
        imag.close();
    } catch (...) {
        cout << "Not ready for processing" << endl;
        exit(1);
    }


    cout << satsensor << endl;

    string link;
    string python;
    //string command = "cp -p ";
    string command;
    string slash = "/";
    string xml0 = ".xml";
    string xml1 = ".xml ";
    string IMAGEDATA = "IMAGEDATA";
    string files = "*.cos ";
    string file = ".cos";

    if (strcmp(satsensor, "TSX") == 0) {
        command = "ln -s ";
        string fn = m + file;
        if (fexists(fn.c_str()) != 1) {
            link = command + sd + is[mcurrent] + slash + IMAGEDATA + slash + files + m + file;
            system(link.c_str());
        }
        fn = s + file;
        if (fexists(fn.c_str()) != 1) {
            link = command + sd + is[scurrent] + slash + IMAGEDATA + slash + files + s + file;
            system(link.c_str());
        }

        fn = m + xml0;

        command = "cp -p ";
        if (fexists(fn.c_str()) != 1) {
            link = command + sd + is[mcurrent] + slash + is[mcurrent] + xml1 + m + xml0;
            system(link.c_str());
        }
        fn = s + xml0;
        if (fexists(fn.c_str()) != 1) {
            link = command + sd + is[scurrent] + slash + is[scurrent] + xml1 + s + xml0;
            system(link.c_str());
            link = command + sd + is[scurrent] + slash + is[scurrent] + xml1 + "leader.xml";
            system(link.c_str());
        }
        command = "tsx_dump_extent.py ";
        string endcommand = ".xml > slave.extent";
        python = command + s + endcommand;
        system(python.c_str());

    } else if (strcmp(satsensor, "ERS1CEOS") == 0 || strcmp(satsensor, "ERS2CEOS") == 0) {
        file = "DAT_01.001 ";
        command = "ln -s ";
        IMAGEDATA = "SCENE1";
        string fn = m + file;
        if (fexists(fn.c_str()) != 1) {

            link = command + sd + is[mcurrent] + slash + IMAGEDATA + slash + file + m + ".DAT";
            cout << link << endl;
            system(link.c_str());
        }

        fn = s + file;
        if (fexists(fn.c_str()) != 1) {

            link = command + sd + is[scurrent] + slash + IMAGEDATA + slash + file + s + ".DAT";
            cout << link << endl;
            system(link.c_str());
        }

        file = "VDF_DAT.001 ";
        fn = m + file;
        if (fexists(fn.c_str()) != 1) {

            link = command + sd + is[mcurrent] + slash + IMAGEDATA + slash + file + m + ".VDF";
            cout << link << endl;
            system(link.c_str());
        }

        fn = s + file;
        if (fexists(fn.c_str()) != 1) {

            link = command + sd + is[scurrent] + slash + IMAGEDATA + slash + file + s + ".VDF";
            cout << link << endl;
            system(link.c_str());
        }
        file = "LEA_01.001 ";
        fn = m + file;
        if (fexists(fn.c_str()) != 1) {

            link = command + sd + is[mcurrent] + slash + IMAGEDATA + slash + file + m + ".LEA";
            cout << link << endl;
            system(link.c_str());
        }

        fn = s + file;
        if (fexists(fn.c_str()) != 1) {

            link = command + sd + is[scurrent] + slash + IMAGEDATA + slash + file + s + ".LEA";
            cout << link << endl;
            system(link.c_str());
        }



    } else if (strcmp(satsensor, "ERS1E1") == 0 || strcmp(satsensor, "ERS2E2") == 0 || strcmp(satsensor, "ASARIMS") == 0) {
        command = "ln -s ";
        string fn = m + file;
        if (fexists(fn.c_str()) != 1) {

            link = command + sd + is[mcurrent] + " " + m + ".N1";
            cout << "INFO: " << link << endl;
            system(link.c_str());
        }
        fn = s + file;
        if (fexists(fn.c_str()) != 1) {

            link = command + sd + is[scurrent] + " " + s + ".N1";
            cout << "INFO: " << link << endl;
            system(link.c_str());
        }




    }




    mkdir("Inputfiles", 0777);
    mkdir("Outdata", 0777);
    mkdir("Outinfo", 0777);
    chdir("Inputfiles");
    if (fexists("m_readfile") == 0) {
        inputfiles(m, s, satsensor, r0, rl, c0, cl);
    }
    chdir("../");


    std::string inputfiles[] = {
        /*  0 */ " -h",
        /*  1 */ " Inputfiles/m_readfiles", //includes porbits if avaliable
        /*  2 */ " Inputfiles/s_readfiles", //includes porbits if avaliable
        /*  3 */ " Inputfiles/coarseorb",
        /*  4 */ " Inputfiles/m_crop;",
        /*    */ //" Inputfiles/m_simamp;",
        /*    */ //" Inputfiles/m_timing;",
        /*  5 */ " Inputfiles/m_ovs;",
        /*  6 */ " Inputfiles/s_crop;",
        /*  7 */ " Inputfiles/s_ovs;",
        /*  8 */ " Inputfiles/coarsecorr;",
        /*    */ //" Inputfiles/m_filtazi;",	
        /*    */ //" Inputfiles/s_filtazi;",
        /*  9 */ " Inputfiles/fine;",
        /*    */ //" Inputfiles/reltiming;",
        /*    */ //" Inputfiles/demassist;",
        /* 10 */ " Inputfiles/coregpm;",
        /*    */ //"SPOTLIGHT_2.exe",			
        /*    */ //"SPOTLIGHT_2",                          
        /*    */ //"matlab -nosplash -nodesktop -r SPOTLIGHT_2",    
        /* 11 */ " Inputfiles/resample;",
        /*    */ //"SPOTLIGHT_3.exe",			
        /*    */ //"SPOTLIGHT_3",                            
        /*    */ //"matlab -nosplash -nodesktop -r SPOTLIGHT_3",   
        /*    */ //" Inputfiles/filtrange;",	
        /* 12 */ " Inputfiles/interfero;",
        /* 13 */ " Inputfiles/comprefpha;",
        /* 14 */ " Inputfiles/subtrrefpha;",
        /* 15 */ " Inputfiles/comprefdem;",
        /* 16 */ " Inputfiles/subtrrefdem;",
        /* 17 */ " Inputfiles/coherence;",
        /* 18 */ " Inputfiles/filtphase;",
        /* 19 */ " Inputfiles/unwrap;",
        /* 20 */ " Inputfiles/dinsar;",
        /* 21 */ " Inputfiles/slant2h;"
        /* 22 */ " Inputfiles/geocode;"

    };


    //if currentstep is oversampling then fix the master.res file!!!
    for (int i = first; i <= last; i++) {
        std::string program = "doris";
        //std::string program = "/home/michael/GeoTools/DEVELOPMENT/doris_v4.06beta/trunk/bin/dorisBeta";
        std::string execute = program += inputfiles[i];
        stepheadline(execute, i);
        cout << execute << endl;
        system(execute.c_str());
        // 7 is the oversampling step.
        if (i == 7) {
            fixmaster();
        }
        if (i == last) {
            endofprocessing();

        }


    }





}
