#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>

std::string filename = "m_readfiles";
using namespace std;

int generalheader(std::string name) {
    //WRITE General Header for DORIS


    ofstream file(name.c_str());
    if (!file) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file << "#   ------------------------------------------------------ // " << endl;
        file << "#   The general options" << endl;
        file << "#   ------------------------------------------------------ // " << endl;
        file << "" << endl;
        file << "SCREEN                         info                        // level of output to standard out" << endl;
        file << "c SCREEN                       debug                       // [trace, debug, info, progress, warning, error]" << endl;
        file << "c SCREEN                       trace                       // [trace, debug, info, progress, warning, error]" << endl;
        file << "BEEP                           warning                     // level of beeping" << endl;
        file << "c PREVIEW                      xv                          // SUNraster files with cpxfiddle" << endl;
        file << "PREVIEW                        on                          // SUNraster files with cpxfiddle" << endl;
        file << "MEMORY                         500                         // [MB] RAM available to Doris" << endl;
        file << "OVERWRITE                      ON                          // do overwrite existing files" << endl;
        file << "BATCH                          ON                          // do process non-interactive" << endl;
        file << "LISTINPUT                      ON                          // do copy this file to log" << endl;
        file << "HEIGHT                         0.0                         // average WGS84 terrain height" << endl;
        file << "TIEPOINT                       0.0 0.0 0.0                 // lat/lon/hei of a tiepoint" << endl;
        file << "ORB_INTERP                     POLYFIT numberofdatapoints-1 // 3rd degree polynomial" << endl;
        file << "c ORB_INTERP                   SPLINE                      // natural cubic spline" << endl;
        file << "c ORB_INTERP                   POLYFIT                     // orbit interpolation method" << endl;
        file << "c ELLIPSOID                    WGS84                       // WGS84 (default), GRS80, BESSEL or define a,b  (major radius, minor radius)" << endl;
        file << "                                                           // " << endl;
        file << "c DUMPBASELINE                 15 10                       // eval baseline on grid" << endl;
        file << "c M_RG_T_ERROR                 0.0                         // one-way [sec] timing error" << endl;
        file << "c M_AZ_T_ERROR                 0.0                         // azimuth timing error" << endl;
        file << "c S_RG_T_ERROR                 0.0                         // one-way [sec]; d_rg=err*rsr*2" << endl;
        file << "c S_AZ_T_ERROR                 0.0                         // [sec]; d_az=err*prf [m]" << endl;
        file << "                                                           // " << endl;
        file << "#   ------------------------------------------------------ // " << endl;
        file << "#   The general io files                                   // " << endl;
        file << "#   ------------------------------------------------------ // " << endl;
        file << "                                                           // " << endl;
        file << "LOGFILE                        doris.log                   // log file" << endl;
        file << "M_RESFILE                      master.res                  // parameter file master" << endl;
        file << "S_RESFILE                      slave.res                   // parameter file slave" << endl;
        file << "I_RESFILE                      master_slave.res            // parameter file interferogram" << endl;
        file << "" << endl;

    }
    file.close();

}

int inputfiles(string m, string s, char* sensor, int r0, int rl, int c0, int cl) {

    string mdate = m;
    string sdate = s;

    // <editor-fold defaultstate="collapsed" desc=" m_readfiles ">
    filename = "m_readfiles";
    generalheader(filename);
    ofstream file1(filename.c_str(), ios::out | ios::app);
    if (!file1) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file1 << "PROCESS                       m_readfiles                 // reading of slc files" << endl;
        if (strcmp(sensor, "ERS1CEOS") == 0 || 
            strcmp(sensor, "ERS2CEOS") == 0) {
            file1 << "PROCESS                       m_porbits                   // get precise orbits" << endl;
        }
        file1 << "                                                          // " << endl;
        file1 << "#   ----------------------------------------------------- // " << endl;
        file1 << "#      READFILES (master)                                 // " << endl;
        file1 << "#   ----------------------------------------------------- // " << endl;
        file1 << "                                                          // " << endl;
        if (strcmp(sensor, "TSX") == 0 ||
            strcmp(sensor, "TSX_SP") == 0) {
            file1 << "M_IN_METHOD                   TSX                         // TERRASAR-X" << endl;
            file1 << "M_IN_DAT                      " << mdate << ".cos         // please link the actual input file with ln -s" << endl;
            file1 << "M_IN_LEA                      " << mdate << ".xml         // " << endl;
        } else if (strcmp(sensor, "ERS1CEOS") == 0 ||
                   strcmp(sensor, "ERS2CEOS") == 0) {
            file1 << "M_IN_METHOD ERS                                           // ERS1CEOS & ERS2CEOS " << endl;
            file1 << "M_IN_VOL                      " << mdate << ".VDF         // VDF" << endl;
            file1 << "M_IN_LEA                      " << mdate << ".LEA         // LEA" << endl;
            file1 << "M_IN_DAT                      " << mdate << ".DAT         // DAT" << endl;
            file1 << "                                                          // " << endl;
            file1 << "#   ----------------------------------------------------- // " << endl;
            file1 << "#      PORBITS (master)                                   // " << endl;
            file1 << "#   ----------------------------------------------------- // " << endl;
            file1 << "                                                         // " << endl;
            if (strcmp(sensor, "ERS1CEOS") == 0) {
                file1 << "M_ORBDIR	               /home/michael/SOURCE/ORBIT/ERS1/dgm-e04   // " << endl; //hardcoded. bad!!!!
            } else if (strcmp(sensor, "ERS2CEOS") == 0) {
                file1 << "M_ORBDIR	               /home/michael/SOURCE/ORBIT/ERS2/ODR   // " << endl; //hardcoded. bad!!!!
            }
            file1 << "M_ORB_EXTRATIME 5                                         // Time before first line" << endl;
            file1 << "M_ORB_INTERVAL  1                                         // Time between data}" << endl;
        } else if (strcmp(sensor, "ERS1E1") == 0 || 
                   strcmp(sensor, "ERS2E2") == 0 || 
                   strcmp(sensor, "ASARIMS") == 0) {
            file1 << "M_IN_METHOD                   ASAR                        // ERS1E1, ERS2E2, ASARIMS  " << endl;
            file1 << "M_IN_DAT                      " << mdate << ".N1          // E1 is similar to N1 and therefore ERS_N1" << endl;
            file1 << "                                                          // " << endl;
            file1 << "#   ----------------------------------------------------- // " << endl;
            file1 << "#      PORBITS (master)                                   // " << endl;
            file1 << "#   ----------------------------------------------------- // " << endl;
            file1 << "                                                          // " << endl;
            if (strcmp(sensor, "ERS1E1") == 0){
                file1 << "M_ORBDIR	               /home/michael/SOURCE/ORBIT/ERS1/dgm-e04   // " << endl; //hardcoded. bad!!!!
            } else if (strcmp(sensor, "ERS2E2") == 0){
                file1 << "M_ORBDIR	               /home/michael/SOURCE/ORBIT/ERS2/ODR   // " << endl; //hardcoded. bad!!!!
            } else if (strcmp(sensor, "ASARIMS") == 0){
                file1 << "M_ORBDIR	               /home/michael/SOURCE/ORBIT/ENVISAT/ODR   // " << endl; //hardcoded. bad!!!!
            }
            file1 << "M_ORB_EXTRATIME              5                            // Time before first line" << endl;
            file1 << "M_ORB_INTERVAL               1                            // Time between data}" << endl;
        }
        file1 << "c                                                         // " << endl;
        file1 << "c                                                         // " << endl;
        file1 << "STOP                                                      // mandatory" << endl;
    }
    file1.close();

    // </editor-fold>                       //define sensor #links to orbit files

    // <editor-fold defaultstate="collapsed" desc=" m_crop ">
    filename = "m_crop";
    generalheader(filename);
    ofstream file2(filename.c_str(), ios::out | ios::app);
    if (!file2) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file2 << "PROCESS                       m_crop                      // convert data to internal format" << endl;
        file2 << "                                                          // " << endl;
        file2 << "#   ----------------------------------------------------- // " << endl;
        file2 << "#      CROP (master)                                      // " << endl;
        file2 << "#   ----------------------------------------------------- // " << endl;
        file2 << "                                                          // " << endl;
        if (strcmp(sensor, "TSX") == 0) {
            file2 << "M_CROP_IN                     " << mdate << ".cos          // please link the actual input file with ln - s " << endl;
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0) {
            file2 << "M_CROP_IN                     " << mdate << ".DAT          // please link the actual input file with ln - s " << endl;
        } else if (strcmp(sensor, "ERS1E1") == 0 || strcmp(sensor, "ERS2E2") == 0 || strcmp(sensor, "ASARIMS") == 0) {
            file2 << "M_CROP_IN                     " << mdate << ".N1          // please link the actual input file with ln - s " << endl;
        }
        file2 << "M_CROP_OUT                    Outdata/master.raw          // output filename" << endl;
        if ((rl == 0) & (cl == 0)) {
            file2 << "c M_DBOW" << endl;
        } else {
            file2 << "M_DBOW                        " << r0 << " " << rl << " " << c0 << " " << cl << "          // in radar coordinates 1000 5500 1000 10000 " << endl;
        }
        file2 << "c                                                         //" << endl;
        file2 << "c                                                         //" << endl;
        file2 << "STOP                                                      // mandatory" << endl;
    }
    file2.close();
    // </editor-fold>                            //define sensor
    
    // <editor-fold defaultstate="collapsed" desc=" m_simamp ">
    filename = "m_simamp";
    generalheader(filename);
    ofstream file26(filename.c_str(), ios::out | ios::app);
    if (!file26) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file26 << "PROCESS                       m_simamp                      // " << endl;
        file26 << "                                                          // " << endl;
        file26 << "#   ----------------------------------------------------- // " << endl;
        file26 << "#      M_SIMAMP                                             // " << endl;
        file26 << "#   ----------------------------------------------------- // " << endl;
        file26 << "                                                          // " << endl;
        file26 << "SAM_IN_FORMAT   I2" << endl;
        file26 << "SAM_IN_DEM      /home/michael/SOURCE/DEM/srtm_61_06/ShanghaiI2.dem" << endl;
        file26 << "SAM_IN_SIZE     6001 6001               // rows cols" << endl;
        file26 << "SAM_IN_DELTA	   0.0008333333 0.000833333    // in degrees " << endl;      
        file26 << "SAM_IN_UL       35.00000 120.00000     // lat and lon of upper left" << endl;
        file26 << "SAM_IN_NODATA   -32768 " << endl; 
        file26 << "SAM_OUT_FILE    Outdata/simamp.dem                      " << endl;
        file26 << "SAM_OUT_DEM     Outdata/dem_sam.dem                     " << endl;
        file26 << "c                                                     //" << endl;
        file26 << "c                                                     //" << endl;
        file26 << "STOP                                        // mandatory" << endl;
    }
    file26.close();
    // </editor-fold>                          //links to DEM file   
    
    // <editor-fold defaultstate="collapsed" desc=" m_timing ">
    filename = "m_timing";
    generalheader(filename);
    ofstream file27(filename.c_str(), ios::out | ios::app);
    if (!file27) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file27 << "PROCESS                       m_timing                      // " << endl;
        file27 << "                                                          // " << endl;
        file27 << "#   ----------------------------------------------------- // " << endl;
        file27 << "#      M_TIMING                                             // " << endl;
        file27 << "#   ----------------------------------------------------- // " << endl;
        file27 << "                                                          // " << endl;
        file27 << "c MTE_METHOD			magfft                          " << endl;
        file27 << "MTE_METHOD			magspace                        " << endl;
        file27 << "MTE_ACC                      128 32                          " << endl;
        file27 << "MTE_NWIN			256                             " << endl;
        file27 << "MTE_WINSIZE			256 128                         " << endl;
        file27 << "MTE_INITOFF			0 0                             " << endl;
        file27 << "c                                                          //" << endl;
        file27 << "c                                                          //" << endl;
        file27 << "STOP                                                       // mandatory" << endl;
    }
    file27.close();
    // </editor-fold>                          //links to DEM file 

    // <editor-fold defaultstate="collapsed" desc=" m_ovs ">
    filename = "m_ovs";
    generalheader(filename);
    ofstream file3(filename.c_str(), ios::out | ios::app);
    if (!file3) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file3 << "PROCESS                       m_ovs                       // optionally oversample data" << endl;
        file3 << "                                                          // " << endl;
        file3 << "#   ----------------------------------------------------- // " << endl;
        file3 << "#      OVERSAMPLE (master)                                // " << endl;
        file3 << "#   ----------------------------------------------------- // " << endl;
        file3 << "                                                          // " << endl;
        file3 << "M_OVS_OUT                     Outdata/master_ovs.raw      // output filename " << endl;
        file3 << "M_OVS_OUT_FORMAT              cr4                         // output format for the oversampled image ci2 | cr4." << endl;
        file3 << "M_OVS_FACT_RNG                1                           // range oversampling ratio" << endl;
        file3 << "M_OVS_FACT_AZI                1                           // azimuth oversampling ratio (>1 not implemented yet!)" << endl;
        file3 << "M_OVS_KERNELSIZE              16                          // interpolation kernel length " << endl;
        file3 << "c                                                         //" << endl;
        file3 << "c                                                         //" << endl;
        file3 << "STOP                                                      // mandatory" << endl;
    }
    file3.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" s_readfiles ">
    filename = "s_readfiles";
    generalheader(filename);
    ofstream file4(filename.c_str(), ios::out | ios::app);

    if (!file4) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file4 << "PROCESS                       s_readfiles                 // reading of slc files" << endl;
        if (strcmp(sensor, "ERS1CEOS") == 0 || 
            strcmp(sensor, "ERS2CEOS") == 0) {
            file4 << "PROCESS                       s_porbits                   // get precise orbits" << endl;
        }
        file4 << "                                                          // " << endl;
        file4 << "#   ----------------------------------------------------- // " << endl;
        file4 << "#      READFILES (slave)                                  // " << endl;
        file4 << "#   ----------------------------------------------------- // " << endl;
        file4 << "                                                          // " << endl;
        if (strcmp(sensor, "TSX") == 0 ||
            strcmp(sensor, "TSX_SP") == 0) {
            file4 << "S_IN_METHOD                   TSX                         // TERRASAR-X" << endl;
            file4 << "S_IN_DAT                      " << sdate << ".cos         // please link the actual input file with ln -s" << endl;
            file4 << "S_IN_LEA                      " << sdate << ".xml         // " << endl;
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0) {
            file4 << "S_IN_METHOD                   ERS                                           // ERS1CEOS " << endl;
            file4 << "S_IN_VOL                      " << sdate << ".VDF         // VDF" << endl;
            file4 << "S_IN_LEA                      " << sdate << ".LEA         // LEA" << endl;
            file4 << "S_IN_DAT                      " << sdate << ".DAT         // DAT" << endl;
            file4 << "                                                          // " << endl;
            file4 << "#   ----------------------------------------------------- // " << endl;
            file4 << "#      PORBITS (slave)                                   // " << endl;
            file4 << "#   ----------------------------------------------------- // " << endl;
            file4 << "                                                          // " << endl;
            if (strcmp(sensor, "ERS1CEOS") == 0) {
                file4 << "S_ORBDIR	  /home/michael/SOURCE/ORBIT/ERS1/dgm-e04   // " << endl; //hardcoded. bad!!!!
            } else if (strcmp(sensor, "ERS2CEOS") == 0) {
                file4 << "S_ORBDIR	  /home/michael/SOURCE/ORBIT/ERS2/ODR   // " << endl; //hardcoded. bad!!!!
            }
            file4 << "S_ORB_EXTRATIME 5                                         // Time before first line" << endl;
            file4 << "S_ORB_INTERVAL  1                                         // Time between data}" << endl;
        } else if (strcmp(sensor, "ERS1E1") == 0 || strcmp(sensor, "ERS2E2") == 0 || strcmp(sensor, "ASARIMS") == 0) {
            file4 << "S_IN_METHOD                   ASAR                                           // ERS1CEOS " << endl;
            file4 << "S_IN_DAT                      " << sdate << ".N1         // DAT" << endl;
            file4 << "                                                          // " << endl;
            file4 << "#   ----------------------------------------------------- // " << endl;
            file4 << "#      PORBITS (slave)                                   // " << endl;
            file4 << "#   ----------------------------------------------------- // " << endl;
            file4 << "                                                          // " << endl;
            if(strcmp(sensor, "ERS1E1") == 0 ){
                file4 << "S_ORBDIR	  /home/michael/SOURCE/ORBIT/ERS1/dgm-e04   // " << endl; //hardcoded. bad!!!!
            }else if (strcmp(sensor, "ERS2E2") == 0){
                file4 << "S_ORBDIR	  /home/michael/SOURCE/ORBIT/ERS1/ODR   // " << endl; //hardcoded. bad!!!!
                        }
            else if (strcmp(sensor, "ASARIMS") == 0){
                file4 << "S_ORBDIR	  /home/michael/SOURCE/ORBIT/ENVISAT/ODR   // " << endl; //hardcoded. bad!!!!
                        }
            file4 << "S_ORB_EXTRATIME 5                                         // Time before first line" << endl;
            file4 << "S_ORB_INTERVAL  1                                         // Time between data}" << endl;
        }
        file4 << "c                                                         // " << endl;
        file4 << "c                                                         // " << endl;
        file4 << "STOP                                                      // mandatory" << endl;
    }
    file4.close();
    // </editor-fold>                       //define sensor

    // <editor-fold defaultstate="collapsed" desc=" s_crop ">
    filename = "s_crop";
    generalheader(filename);
    ofstream file5(filename.c_str(), ios::out | ios::app);

    if (!file5) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        int ex = 400;
        file5 << "PROCESS                       s_crop                      // convert data to internal format" << endl;
        file5 << "                                                          // " << endl;
        file5 << "#   ----------------------------------------------------- // " << endl;
        file5 << "#      CROP (slave)                                       // " << endl;
        file5 << "#   ----------------------------------------------------- // " << endl;
        file5 << "                                                          // " << endl;
        if (strcmp(sensor, "TSX") == 0 || strcmp(sensor, "TSX_SP") == 0) {
            file5 << "S_CROP_IN                     " << sdate << ".cos          // please link the actual input file with ln - s " << endl;
        } else if (strcmp(sensor, "ERS1CEOS") == 0 ||
                   strcmp(sensor, "ERS2CEOS") == 0) {
            file5 << "S_CROP_IN                     " << sdate << ".DAT          // please link the actual input file with ln - s " << endl;
        } else if (strcmp(sensor, "ERS1E1") == 0 ||
                   strcmp(sensor, "ERS2E2") == 0 || 
                   strcmp(sensor, "ASARIMS") == 0) {
            file5 << "S_CROP_IN                     " << sdate << ".N1          // please link the actual input file with ln - s " << endl;
        }
        file5 << "S_CROP_OUT                    Outdata/slave.raw           // output filename" << endl;
        file5 << "c S_DBOW (extent by " << ex << " pixels for coreg): " << r0 << " " << rl << " " << c0 << " " << cl << endl;

        int li, pi;
        if (strcmp(sensor, "TSX") == 0 || strcmp(sensor, "TSX_SP") == 0) {
            // Read extent from slave.extent	
            std::ifstream file("../slave.extent");
            std::string lines;
            std::string pixels;
            std::getline(file, lines);
            std::getline(file, pixels);
            li = atoi(lines.c_str());
            pi = atoi(pixels.c_str());
            file.close();
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0) {
            li = 26000; // hard coded dimensions of ERS1 (have to find a way to extract lines and pixels first)
            pi = 4900;
            ex = 0;
        } else if (strcmp(sensor, "ERS1E1") == 0 || strcmp(sensor, "ERS2E2") == 0) {
            li = 26000; // hard coded dimensions of ERS1 (have to find a way to extract lines and pixels first)
            pi = 4900;
            ex = 0;
        } else if (strcmp(sensor, "ASARIMS") ==0) {
            li = 27000; // hard coded dimensions of ERS1 (have to find a way to extract lines and pixels first)
            pi = 5100;
            ex = 0;
        }
        
        
        
        
        if ((rl == 0) & (cl == 0)) {
            file5 << "c S_DBOW" << endl;
        } else {
            if ((r0 - ex) < 0) {
                r0 = 1;
            } else {
                r0 = r0 - ex;
            }

            if ((rl + ex) > li) {
                rl = li;
            } else {
                rl = rl + ex;
            }

            if ((c0 - ex) < 0) {
                c0 = 1;
            } else {
                c0 = c0 - ex;
            }

            if ((cl + ex) > pi) {
                cl = pi;
            } else {
                cl = cl + ex;
            }
            file5 << "S_DBOW                        " << r0 << " " << rl << " " << c0 << " " << cl << "          // in radar coordinates 1000 5500 1000 10000 " << endl;
        }
        file5 << "c                                                         // " << endl;
        file5 << "c                                                         // " << endl;
        file5 << "STOP                                                      // mandatory" << endl;
    }
    file5.close();
    // </editor-fold>                            //define sensor and dimensions of regular image

    // <editor-fold defaultstate="collapsed" desc=" s_ovs ">
    filename = "s_ovs";
    generalheader(filename);
    ofstream file6(filename.c_str(), ios::out | ios::app);

    if (!file6) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file6 << "PROCESS                       s_ovs                       // optionally oversample data" << endl;
        file6 << "                                                          // " << endl;
        file6 << "#   ----------------------------------------------------- // " << endl;
        file6 << "#      OVERSAMPLE (slave)                                 // " << endl;
        file6 << "#   ----------------------------------------------------- // " << endl;
        file6 << "                                                          // " << endl;
        file6 << "S_OVS_OUT                     Outdata/slave_ovs.raw       // output filename" << endl;
        file6 << "S_OVS_OUT_FORMAT              cr4                         // output format for the oversampled image ci2 | cr4." << endl;
        file6 << "S_OVS_FACT_RNG                1     			    // range oversampling ratio" << endl;
        file6 << "S_OVS_FACT_AZI                1     			    // azimuth oversampling ratio (>1 not implemented yet!)" << endl;
        file6 << "S_OVS_KERNELSIZE              16    			    // interpolation kernel length" << endl;
        file6 << "c                                                         // " << endl;
        file6 << "c                                                         // " << endl;
        file6 << "STOP                                                      // mandatory" << endl;
    }
    file6.close();
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" coarseorb ">
    filename = "coarseorb";
    generalheader(filename);
    ofstream file7(filename.c_str(), ios::out | ios::app);

    if (!file7) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {


        file7 << "PROCESS                       coarseorb                   // no cards later" << endl;
        file7 << "                                                          // " << endl;
        file7 << "#   ----------------------------------------------------- // " << endl;
        file7 << "#      COARSE Orbits					    // " << endl;
        file7 << "#   ----------------------------------------------------- // " << endl;
        file7 << "                                                          // " << endl;
        file7 << "c                                                         // " << endl;
        file7 << "c                                                         // " << endl;
        file7 << "STOP                                                      // mandatory" << endl;
    }
    file7.close();
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" coarsecorr ">
    filename = "coarsecorr";
    generalheader(filename);
    ofstream file8(filename.c_str(), ios::out | ios::app);

    if (!file8) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file8 << "PROCESS                       coarsecorr                  // based on correlation" << endl;
        file8 << "                                                          // " << endl;
        file8 << "#   ----------------------------------------------------- // " << endl;
        file8 << "#      COARSE COREGISTRATION                              // " << endl;
        file8 << "#   ----------------------------------------------------- // " << endl;
        file8 << "                                                          // " << endl;
        file8 << "c CC_METHOD                   magfft                      // default" << endl;
        file8 << "CC_METHOD                     magspace                    // " << endl;
        file8 << "CC_NWIN                       21                          // number of large windows" << endl;
        file8 << "CC_WINSIZE                    256 256                     // size of windows (square better?)" << endl;
        file8 << "c CC_INITOFF                  0 0                         // initial offset" << endl;
        file8 << "CC_INITOFF                    orbit                       // use result of orbits" << endl;
        file8 << "c                                                         // " << endl;
        file8 << "c                                                         // " << endl;
        file8 << "STOP                                                      // mandatory" << endl;
    }
    file8.close();
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" fine ">
    filename = "fine";
    generalheader(filename);
    ofstream file9(filename.c_str(), ios::out | ios::app);

    if (!file9) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file9 << "PROCESS                       fine                        // fine coregistration" << endl;
        file9 << "                                                          // " << endl;
        file9 << "#   ----------------------------------------------------- // " << endl;
        file9 << "#      FINE COREGISTRATION                                // " << endl;
        file9 << "#   ----------------------------------------------------- // " << endl;
        file9 << "                                                          // " << endl;
        file9 << "FC_METHOD                     magfft                      // fast" << endl;
        file9 << "c FC_METHOD                   oversample                  // best: oversample complex data" << endl;
        file9 << "c FC_METHOD                   magspace                    // same as magfft" << endl;
        file9 << "FC_NWIN                       601                         // number of smaller windows" << endl;
        file9 << "c FC_IN_POS                   highcorr.pos                // filename with window positions" << endl;
        file9 << "FC_WINSIZE                    128 128                       // size of windows" << endl;
        file9 << "FC_ACC                        16 16                         // search window, 2^n" << endl;
        file9 << "FC_INITOFF                    coarsecorr                  // use result of coarse" << endl;
        file9 << "FC_OSFACTOR                   32                          // oversampling factor" << endl;
        file9 << "c FC_PLOT                       0.2 NOBG                    // GMT plot with magnitude background" << endl;
        file9 << "c                                                         // " << endl;
        file9 << "c                                                         // " << endl;
        file9 << "STOP                                                      // mandatory" << endl;
    }
    file9.close();
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" coregpm ">
    filename = "coregpm";
    generalheader(filename);
    ofstream file10(filename.c_str(), ios::out | ios::app);

    if (!file10) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file10 << "PROCESS                       coregpm                     // estimate coregistration param." << endl;
        file10 << "                                                          // " << endl;
        file10 << "#   ----------------------------------------------------- // " << endl;
        file10 << "#      COREGISTRATION PARAMETERS MODELING                 // " << endl;
        file10 << "#   ----------------------------------------------------- // " << endl;
        file10 << "                                                          // " << endl;
        file10 << "CPM_THRESHOLD                 0.3                         // higher threshold->less windows" << endl;
        file10 << "CPM_DEGREE                    2                           // 2d-polynomial" << endl;
        file10 << "c CPM_DEGREE                  1                           // 1d-polynomial (if demassist coregistration)" << endl;
        file10 << "c CPM_WEIGHT                  linear                      // or [none]" << endl;
        file10 << "c CPM_WEIGHT                  quadratic                   // or [none]" << endl;
        file10 << "CPM_WEIGHT                    bamler                      // paper R.Bamler, IGARSS 2000/2004" << endl;
        file10 << "CPM_MAXITER                   10000                       // automated removal, max 10000 iterations" << endl;
        file10 << "c CPM_K_ALPHA                 1.97                        // automated removal crit. value" << endl;
        file10 << "c CPM_PLOT                      NOBG                        // plot w/o magnitude background" << endl;
        file10 << "CPM_DUMP                      OFF                          // no creation of files" << endl;
        file10 << "c                                                         // " << endl;
        file10 << "c                                                         // " << endl;
        file10 << "STOP                                                      // mandatory" << endl;
    }
    file10.close();
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" resample ">
    filename = "resample";
    generalheader(filename);
    ofstream file11(filename.c_str(), ios::out | ios::app);

    if (!file11) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file11 << "PROCESS                       resample                      // slave" << endl;
        file11 << "                                                            // " << endl;
        file11 << "#   ------------------------------------------------------- // " << endl;
        file11 << "#      RESAMPLING SLAVE                                     // " << endl;
        file11 << "#   ------------------------------------------------------- // " << endl;
        file11 << "                                                            // " << endl;
        if (strcmp(sensor, "TSX_SP") == 0) {
            file11 << "RS_METHOD                     cc6p_SP                   // best choice (theory 0.9999)" << endl;

        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0 || strcmp(sensor, "TSX") == 0) {
            file11 << "RS_METHOD                     cc6p                          //" << endl;
        }
        file11 << "RS_OUT_FILE                   Outdata/slave.resampled       // " << endl;
        file11 << "RS_OUT_FORMAT                 cr4                           // " << endl;
        if (strcmp(sensor, "TSX") == 0 || strcmp(sensor, "TSX_SP") == 0) {
            file11 << "RS_SHIFTAZI                   off                           // keep this on" << endl;
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0) {
            file11 << "RS_SHIFTAZI                   on                           // keep this on" << endl;
        }
        file11 << "c RS_DBOW                     1000 5500 1000 10000          // " << endl;
        file11 << "c                                                           // " << endl;
        file11 << "c                                                           // " << endl;
        file11 << "STOP                                                        // mandatory" << endl;
    }
    file11.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" m_filtazi ">
    filename = "m_filtazi";
    generalheader(filename);
    ofstream file12(filename.c_str(), ios::out | ios::app);

    if (!file12) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file12 << "PROCESS                       m_filtazi                   // azimuth filter" << endl;
        file12 << "                                                          // " << endl;
        file12 << "#   ----------------------------------------------------- // " << endl;
        file12 << "#      MASTER AZIMUTH FILTERING                                  // " << endl;
        file12 << "#   ----------------------------------------------------- // " << endl;
        file12 << "                                                          // " << endl;
        file12 << "c AF_METHOD                                               // " << endl;
        file12 << "AF_BLOCKSIZE                  1024                        // fftlength each column" << endl;
        file12 << "AF_OVERLAP                    64                          // hbs" << endl;
        file12 << "AF_HAMMING                    0.75                        // alpha of hamming, see logfile" << endl;
        file12 << "AF_OUT_MASTER                 Outdata/master.af           // " << endl;
        file12 << "AF_OUT_FORMAT                 cr4                         // " << endl;
        file12 << "c                                                         // " << endl;
        file12 << "c                                                         // " << endl;
        file12 << "STOP                                                      // mandatory" << endl;
    }
    file12.close();

    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc=" s_filtazi ">
    filename = "s_filtazi";
    generalheader(filename);
    ofstream file25(filename.c_str(), ios::out | ios::app);

    if (!file25) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file25 << "PROCESS                       s_filtazi                   // azimuth filter" << endl;
        file25 << "                                                          // " << endl;
        file25 << "#   ----------------------------------------------------- // " << endl;
        file25 << "#      SLAVE AZIMUTH FILTERING                            // " << endl;
        file25 << "#   ----------------------------------------------------- // " << endl;
        file25 << "                                                          // " << endl;
        file25 << "c AF_METHOD                                               // " << endl;
        file25 << "AF_BLOCKSIZE                  1024                        // fftlength each column" << endl;
        file25 << "AF_OVERLAP                    64                          // hbs" << endl;
        file25 << "AF_HAMMING                    0.75                        // alpha of hamming, see logfile" << endl;
        file25 << "AF_OUT_SLAVE                  Outdata/slave.af            // " << endl;
        file25 << "AF_OUT_FORMAT                 cr4                         // " << endl;
        file25 << "c                                                         // " << endl;
        file25 << "c                                                         // " << endl;
        file25 << "STOP                                                      // mandatory" << endl;
    }
    file25.close();

    // </editor-fold>           

    // <editor-fold defaultstate="collapsed" desc=" filtrange ">
    filename = "filtrange";
    generalheader(filename);
    ofstream file13(filename.c_str(), ios::out | ios::app);

    if (!file13) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file13 << "PROCESS                       filtrange                   // azimuth filter" << endl;
        file13 << "                                                          // " << endl;
        file13 << "#   ----------------------------------------------------- // " << endl;
        file13 << "#      RANGE FILTERING (master&slave)                     // " << endl;
        file13 << "#   ----------------------------------------------------- // " << endl;
        file13 << "                                                          // " << endl;
        file13 << "RF_METHOD                     porbits                     // based on orbit geometry" << endl;
        file13 << "RF_SLOPE                      0.0                         // [deg] and terrain slope" << endl;
        file13 << "RF_FFTLENGTH                  1024                        // power of 2" << endl;
        file13 << "RF_HAMMING                    0.75                        // alpha" << endl;
        file13 << "RF_OUT_MASTER                 Outdata/master.rf_orb       // " << endl;
        file13 << "RF_OUT_SLAVE                  Outdata/slave.rf_orb        // " << endl;
        file13 << "RF_OUT_FORMAT                 cr4                         // " << endl;
        file13 << "c                                                         // " << endl;
        file13 << "c                                                         // " << endl;
        file13 << "STOP                                                      // mandatory" << endl;
    }
    file13.close();
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" interfero ">
    filename = "interfero";
    generalheader(filename);
    ofstream file14(filename.c_str(), ios::out | ios::app);

    if (!file14) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file14 << "PROCESS                       interfero                     // " << endl;
        file14 << "                                                            // " << endl;
        file14 << "#   ------------------------------------------------------- // " << endl;
        file14 << "#      INTERFEROGRAM                                        // " << endl;
        file14 << "#   ------------------------------------------------------- // " << endl;
        file14 << "                                                            // " << endl;
        file14 << "INT_METHOD                    old                           // " << endl;
        file14 << "c INT_METHOD                  oversample                    // " << endl;
        file14 << "INT_OUT_CINT                  Outdata/master_slave.cint     // optional complex int." << endl;
        file14 << "c INT_OUT_INT                 Outdata/master_slave.int      // optional real int." << endl;
        if (strcmp(sensor, "TSX") == 0  || strcmp(sensor, "TSX_SP") == 0) {
            file14 << "INT_MULTILOOK                 1 1                           // line, pixel factors" << endl;
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0 || strcmp(sensor, "ASARIMS") == 0) {
            file14 << "INT_MULTILOOK                 5 1                           // line, pixel factors" << endl;
        }
        file14 << "c                                                           // " << endl;
        file14 << "c                                                           // " << endl;
        file14 << "STOP                                                        // mandatory" << endl;
    }
    file14.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" comprefpha ">
    filename = "comprefpha";
    generalheader(filename);
    ofstream file15(filename.c_str(), ios::out | ios::app);

    if (!file15) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file15 << "PROCESS                       comprefpha                    // estimate flatearth param." << endl;
        file15 << "                                                            // " << endl;
        file15 << "#   ------------------------------------------------------- // " << endl;
        file15 << "#      REFERENCE PHASE COMPUTATION                          // " << endl;
        file15 << "#   ------------------------------------------------------- // " << endl;
        file15 << "                                                            // " << endl;
        file15 << "FE_METHOD                     porbits                       // " << endl;
        file15 << "c FE_METHOD                   method2                       // not implemented?" << endl;
        file15 << "FE_DEGREE                     5                             // " << endl;
        file15 << "FE_NPOINTS                    501                           // " << endl;
        file15 << "c                                                           // " << endl;
        file15 << "c                                                           // " << endl;
        file15 << "STOP                                                        // mandatory" << endl;
    }
    file15.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" subtrrefpha ">
    filename = "subtrrefpha";
    generalheader(filename);
    ofstream file16(filename.c_str(), ios::out | ios::app);

    if (!file16) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file16 << "PROCESS                       subtrrefpha                   // " << endl;
        file16 << "                                                            // " << endl;
        file16 << "#   ------------------------------------------------------- // " << endl;
        file16 << "#      SUBTRACT REFERENCE PHASE                             // " << endl;
        file16 << "#   ------------------------------------------------------- // " << endl;
        file16 << "                                                            // " << endl;
        file16 << "SRP_METHOD                    exact                         // compute foreach pixel" << endl;
        file16 << "c SRP_METHOD                  polynomial                    // evaluate comprefpha poly" << endl;
        file16 << "SRP_OUT_CINT                  Outdata/master_slave.srp      // " << endl;
        file16 << "SRP_MULTILOOK                 1 1                           // " << endl;
        file16 << "c SRP_DUMPREFPHA              ON                            // " << endl;
        file16 << "c SRP_OUT_REFPHA              Outdata/refphase.cr4          // only output for debug" << endl;
        file16 << "c                                                           // " << endl;
        file16 << "c                                                           // " << endl;
        file16 << "STOP                                                        // mandatory" << endl;
    }
    file16.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" coherence ">
    filename = "coherence";
    generalheader(filename);
    ofstream file17(filename.c_str(), ios::out | ios::app);

    if (!file17) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file17 << "PROCESS                       coherence                     // " << endl;
        file17 << "                                                            // " << endl;
        file17 << "#   ------------------------------------------------------- // " << endl;
        file17 << "#      COHERENCE                                            // " << endl;
        file17 << "#   ------------------------------------------------------- // " << endl;
        file17 << "                                                            // " << endl;
        file17 << "COH_METHOD                    refphase_only                 // " << endl;
        file17 << "c COH_METHOD                  include_refdem                // remove topographic phase also." << endl;
        file17 << "c COH_OUT_CCOH                Outdata/master_slave.ccoh     // optional complex image" << endl;
        file17 << "COH_OUT_COH                   Outdata/master_slave.coh      // optional real" << endl;
        if (strcmp(sensor, "TSX") == 0) {
            file17 << "COH_MULTILOOK                 1 1                           // same as interferogram" << endl;
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0 || strcmp(sensor, "ASARIMS") == 0) {
            file17 << "COH_MULTILOOK                 5 1                           // same as interferogram" << endl;

        }
        if (strcmp(sensor, "TSX") == 0) {
            file17 << "COH_WINSIZE                   4 4                          // estimator window size" << endl;
        } else if (strcmp(sensor, "ERS1CEOS") == 0 || strcmp(sensor, "ERS2CEOS") == 0 || strcmp(sensor, "ASARIMS") == 0) {
            file17 << "COH_WINSIZE                   10 2                          // estimator window size" << endl;

        }
        file17 << "c                                                           // " << endl;
        file17 << "c                                                           // " << endl;
        file17 << "STOP                                                        // mandatory" << endl;
    }
    file17.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" filtphase ">
    filename = "filtphase";
    generalheader(filename);
    ofstream file18(filename.c_str(), ios::out | ios::app);

    if (!file18) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file18 << "PROCESS                       filtphase                     // filter interferogram" << endl;
        file18 << "                                                            // " << endl;
        file18 << "#   ------------------------------------------------------- // " << endl;
        file18 << "#      PHASE FILTERING                                      // " << endl;
        file18 << "#   ------------------------------------------------------- // " << endl;
        file18 << "                                                            // " << endl;
        file18 << "c PF_IN_FILE                  Outdata/master_slave.srp 323  // only for standalone usage" << endl;
        file18 << "PF_OUT_FILE                   Outdata/master_slave.cint.filtered    //" << endl;
        file18 << "c PF_METHOD                     spatialconv                   // " << endl;
        file18 << "c PF_KERNEL                     5 1 1 1 1 1                   // " << endl;
        file18 << "c c PF_KERNEL                 5 1 4 9 4 1                   // " << endl;
        file18 << "c c PF_KERNEL                 5 1 2 3 2 1                   // " << endl;
        file18 << "c c PF_KERNEL                 3 -1 1 0                      // " << endl;
        file18 << "c c PF_IN_KERNEL2D            myfilt.asc                    // " << endl;
        file18 << "                                                            // " << endl;
        file18 << "PF_METHOD                   goldstein                     // " << endl;
        file18 << "PF_ALPHA                    0.5                           // " << endl;
        file18 << "PF_BLOCKSIZE                32                            // " << endl;
        file18 << "PF_OVERLAP                  4                            // " << endl;
        file18 << "PF_KERNEL                   5 1 1 1 1 1                   // " << endl;
        file18 << "c                                                           //" << endl;
        file18 << "c                                                           //" << endl;
        file18 << "STOP                                                        // mandatory" << endl;
    }
    file18.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" unwrap ">
    filename = "unwrap";
    generalheader(filename);
    ofstream file19(filename.c_str(), ios::out | ios::app);

    if (!file19) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file19 << "PROCESS                       unwrap                        // unwrap interferogram" << endl;
        file19 << "                                                            // " << endl;
        file19 << "#   ------------------------------------------------------- // " << endl;
        file19 << "#      PHASE UNWRAPPING                                     // " << endl;
        file19 << "#   ------------------------------------------------------- // " << endl;
        file19 << "                                                            // " << endl;
        file19 << "c UW_METHOD                   RAMON                         // (delft only)" << endl;
        file19 << "c UW_SEEDS                    100                           // delta seedL = seedP" << endl;
        file19 << "c UW_SEEDS                    100 200                       // " << endl;
        file19 << "c UW_SEEDS                    seedfilename                  // put EOL after last" << endl;
        file19 << "UW_OUT_FILE                   Outdata/master_slave.uint     // unwrapped interferogram" << endl;
        file19 << "c UW_OUT_REGIONS              Outdata/master_slave.regions  // 2b used with manuw.m" << endl;
        file19 << "UW_METHOD                     SNAPHU                        // " << endl;
        file19 << "UW_OUT_FORMAT                 REAL4                           // " << endl;
        file19 << "UW_SNAPHU_LOG                 snaphu.log                    // " << endl;
        file19 << "UW_SNAPHU_coh                 Outdata/master_slave.coh      // " << endl;
        file19 << "UW_SNAPHU_MODE                TOPO                          // " << endl;
        file19 << "UW_SNAPHU_INIT                MST                           // " << endl;
        file19 << "UW_SNAPHU_VERBOSE             ON                            // " << endl;
        file19 << "c                                                           //" << endl;
        file19 << "c                                                           //" << endl;
        file19 << "STOP                                                        // mandatory" << endl;
    }
    file19.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" dinsar ">
    filename = "dinsar";
    generalheader(filename);
    ofstream file20(filename.c_str(), ios::out | ios::app);
    if (!file20) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file20 << "PROCESS                     dinsar                        // differential interferogram" << endl;
        file20 << "                                                        // " << endl;
        file20 << "" << endl;
        file20 << "#   ------------------------------------------------------- // " << endl;
        file20 << "#      DIFFERENTIAL INSAR                                   // " << endl;
        file20 << "#   ------------------------------------------------------- // " << endl;
        file20 << "                                                       // " << endl;
        file20 << "DI_OUT_FILE                   Outdata/master_slave.dcint    // differential" << endl;
        file20 << "DI_IN_TOPOSLAVE               topodir/slave.res             // " << endl;
        file20 << "DI_IN_TOPOINT                topodir/master_slave.res          // " << endl;
        file20 << "c DI_IN_TOPOMASTER            master.res            // 4 pass" << endl;
        file20 << "c DI_OUT_SCALED               debug.raw                     // " << endl;
        file20 << "c                                                           //" << endl;
        file20 << "c                                                           //" << endl;
        file20 << "STOP                                                        // mandatory" << endl;
    }
    file20.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" slant2h ">
    filename = "slant2h";
    generalheader(filename);
    ofstream file21(filename.c_str(), ios::out | ios::app);

    if (!file21) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file21 << "PROCESS                     slant2h                        // slant 2 height" << endl;
        file21 << "                                                        // " << endl;
        file21 << "" << endl;
        file21 << "#   ------------------------------------------------------- // " << endl;
        file21 << "#      SLANT TO HEIGHT                                   // " << endl;
        file21 << "#   ------------------------------------------------------- // " << endl;
        file21 << "comment ___ SLANT 2 HEIGHT CONVERSION ___			" << endl;
        file21 << "" << endl;
        file21 << "S2H_METHOD schwabisch" << endl;
        file21 << "S2H_NPOINTS 500" << endl;
        file21 << "S2H_DEGREE1D 2" << endl;
        file21 << "S2H_NHEIGHTS 3" << endl;
        file21 << "S2H_DEGREE2D 5" << endl;
        file21 << "S2H_OUT_HEI Outdata/hei.schw" << endl;
        file21 << "c" << endl;
        file21 << "c S2H_METHOD ambiguity" << endl;
        file21 << "c S2H_OUT_HEI Outdata/hei.ambi" << endl;
        file21 << "c S2H_OUT_LAM Outdata/lam.ambi" << endl;
        file21 << "c S2H_OUT_PHI Outdata/phi.ambi" << endl;
        file21 << "c" << endl;
        file21 << "c S2H_METHOD rodriguez" << endl;
        file21 << "c S2H_OUT_HEI Outdata/hei.rodr   " << endl;
        file21 << "c                                                           //" << endl;
        file21 << "c                                                           //" << endl;
        file21 << "STOP                                                        // mandatory" << endl;
    }
    file21.close();

    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc=" geocode ">
    filename = "geocode";
    generalheader(filename);
    ofstream file24(filename.c_str(), ios::out | ios::app);

    if (!file24) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file24 << "PROCESS                     geocode                         // geocode" << endl;
        file24 << "                                                            // " << endl;
        file24 << "" << endl;
        file24 << "#   ------------------------------------------------------- // " << endl;
        file24 << "#      GEOCODE                                              // " << endl;
        file24 << "#   ------------------------------------------------------- // " << endl;
        file24 << "comment ___ GEOCODING ___" << endl;
        file24 << "c" << endl;
        file24 << "GEO_OUT_LAM Outdata/lam.raw" << endl;
        file24 << "GEO_OUT_PHI Outdata/phi.raw" << endl;
        file24 << "c                                                           //" << endl;
        file24 << "c                                                           //" << endl;
        file24 << "STOP                                                        // mandatory" << endl;
    }
    file24.close();

    // </editor-fold>
 
    // <editor-fold defaultstate="collapsed" desc=" comprefdem ">
    filename = "comprefdem";
    generalheader(filename);
    ofstream file22(filename.c_str(), ios::out | ios::app);

    if (!file22) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file22 << "PROCESS                       comprefdem                    // estimate flatearth param." << endl;
        file22 << "                                                            // " << endl;
        file22 << "#   ------------------------------------------------------- // " << endl;
        file22 << "#      REFERENCE DEM                                        // " << endl;
        file22 << "#   ------------------------------------------------------- // " << endl;
        file22 << "CRD_IN_FORMAT   I2" << endl;
        file22 << "CRD_IN_DEM      /home/michael/SOURCE/DEM/srtm_61_06/ShanghaiI2.dem" << endl;
        file22 << "CRD_IN_SIZE     6001 6001               // rows cols" << endl;
        file22 << "CRD_IN_DELTA    0.0008333333 0.000833333    // in degrees " << endl;      
        file22 << "CRD_IN_UL       35.00000 120.00000     // lat and lon of upper left" << endl;
        file22 << "CRD_IN_NODATA   -32768" << endl;
        file22 << "CRD_INCLUDE_FE  OFF                     // phase w.r.t. ellipsoid " << endl;         
        file22 << "CRD_OUT_DEM_LP  dem_radar.raw" << endl;
        file22 << "c                                                           //       " << endl;
        file22 << "c                                                           //       " << endl;
        file22 << "STOP                                                        // mandatory" << endl;

    }
    file22.close();

    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc=" subtrrefdem ">
    filename = "subtrrefdem";
    generalheader(filename);
    ofstream file23(filename.c_str(), ios::out | ios::app);

    if (!file23) {
        cout << "Cannot open output file.\n";
        return 1;
    } else {
        file23 << "PROCESS                       subtrrefdem                    // estimate flatearth param." << endl;
        file23 << "                                                            // " << endl;
        file23 << "#   ------------------------------------------------------- // " << endl;
        file23 << "#      Subtrref REFERENCE DEM                                        // " << endl;
        file23 << "#   ------------------------------------------------------- // " << endl;
        file23 << "SRD_OUT_CINT                  Outdata/cint.minrefdem.raw    // " << endl;
        file23 << "SRD_OFFSET                    0 0                           // " << endl;
        file23 << "c " << endl;
        file23 << "c " << endl;
        file23 << "STOP " << endl;

    }
    file23.close();

    // </editor-fold>

    return 0;
}


