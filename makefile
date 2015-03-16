## -*- Makefile -*-
##
## User: michael
## Time: Aug 9, 2012 9:02:05 AM
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# Define the target directories.
TARGETDIR_stackingL=bin


all: $(TARGETDIR_stackingL)/stackingL

## Target: stackingL

OBJS_stackingL =  \
	$(TARGETDIR_stackingL)/inputfiles.o \
	$(TARGETDIR_stackingL)/main.o \
	$(TARGETDIR_stackingL)/makemaster.o \
	$(TARGETDIR_stackingL)/makepair.o \
	$(TARGETDIR_stackingL)/tools.o
USERLIBS_stackingL = $(SYSLIBS_stackingL) 
DEPLIBS_stackingL =  
LDLIBS_stackingL = $(USERLIBS_stackingL)


# Link or archive
$(TARGETDIR_stackingL)/stackingL: $(TARGETDIR_stackingL) $(OBJS_stackingL) $(DEPLIBS_stackingL)
	$(LINK.cc) $(CCFLAGS_stackingL) $(CPPFLAGS_stackingL) -o $@ $(OBJS_stackingL) $(LDLIBS_stackingL)


# Compile source files into .o files
$(TARGETDIR_stackingL)/inputfiles.o: $(TARGETDIR_stackingL) inputfiles.cpp
	$(COMPILE.cc) $(CCFLAGS_stackingL) $(CPPFLAGS_stackingL) -o $@ inputfiles.cpp

$(TARGETDIR_stackingL)/main.o: $(TARGETDIR_stackingL) main.cpp
	$(COMPILE.cc) $(CCFLAGS_stackingL) $(CPPFLAGS_stackingL) -o $@ main.cpp

$(TARGETDIR_stackingL)/makemaster.o: $(TARGETDIR_stackingL) makemaster.cpp
	$(COMPILE.cc) $(CCFLAGS_stackingL) $(CPPFLAGS_stackingL) -o $@ makemaster.cpp

$(TARGETDIR_stackingL)/makepair.o: $(TARGETDIR_stackingL) makepair.cpp
	$(COMPILE.cc) $(CCFLAGS_stackingL) $(CPPFLAGS_stackingL) -o $@ makepair.cpp

$(TARGETDIR_stackingL)/tools.o: $(TARGETDIR_stackingL) tools.cpp
	$(COMPILE.cc) $(CCFLAGS_stackingL) $(CPPFLAGS_stackingL) -o $@ tools.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_stackingL)/stackingL \
		$(TARGETDIR_stackingL)/inputfiles.o \
		$(TARGETDIR_stackingL)/main.o \
		$(TARGETDIR_stackingL)/makemaster.o \
		$(TARGETDIR_stackingL)/makepair.o \
		$(TARGETDIR_stackingL)/tools.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_stackingL)


# Create the target directory (if needed)
$(TARGETDIR_stackingL):
	mkdir -p $(TARGETDIR_stackingL)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

