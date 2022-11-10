# Make command to use for dependencies
RM=rm
MKDIR=mkdir

APP=test
LIB=-lpthread -lrt -ldl

OBJDIR=../_obj/$(APP)
EXEDIR=../bin
INCDIR=-I../common
COMMON=../common
COMPILE_ARGS=-c -o
LINK_ARGS=-o
LIBDIR=
PLATS=debug release

EXEFILE=$(EXEDIR)/$(APP)
SRC := $(wildcard *.cpp $(COMMON)/*.cpp)

OBJFILE := $(patsubst %.cpp,  $(OBJDIR)/%.o, $(notdir ${SRC}))

COMPILE=g++ $(COMPILE_ARGS) "$(OBJDIR)/$(*F).o" -w "$<"
LINK=g++ $(LIBDIR) $(LINK_ARGS) "$(EXEFILE)" $(OBJFILE) $(OBJFILE_Z)

# Pattern rules
$(OBJDIR)/%.o : %.cpp
	$(COMPILE)
	
$(OBJDIR)/%.o : $(COMMON)/%.cpp
	$(COMPILE)
	
# Build rules
all: $(EXEFILE)
release: $(EXEFILE)
debug: 
	$(MAKE) $(all) COMPILE_ARGS="-g -c -o" LINK_ARGS="-g -o"

$(EXEFILE): $(OBJDIR) $(OBJFILE) $(OBJFILE_Z)
	$(MKDIR) -p $(EXEDIR)
	$(LINK)
$(OBJDIR):
	$(MKDIR) -p $(OBJDIR)


# Rebuild this project
rebuild: cleanall all

# Clean this project
clean:
	$(RM) -f $(EXEFILE)
	$(RM) -f $(OBJFILE)
	$(RM) -f $(OBJFILE_Z)

# Clean this project and all dependencies
cleanall: clean

