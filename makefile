
DEBUG	:=	0

CXX	:= g++
LD	:= g++

CXXFLAGS	:= -std=c++11 -Wall -Wextra -pedantic -fno-rtti -fno-exceptions
LIBS		:= 

ifeq (1, $(DEBUG))
	CXXFLAGS := $(CXXFLAGS) -g -O0 -D_DEBUG
else
	CXXFLAGS := $(CXXFLAGS) -O3 -fomit-frame-pointer
endif

ifeq ($(OS),Windows_NT)
	TARGET	:= $(shell basename $(CURDIR)).exe
else
	TARGET	:= $(shell basename $(CURDIR))
endif
OUTPUT		= $(CURDIR)/$(TARGET)
SOURCES		:= src
INCLUDES	:= 

CXXFILES	:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
OFILES		:= $(CXXFILES:.cpp=.o)
PRECOMPILED	:= src/precompiled.hpp.gch

export VPATH	:= $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export INCLUDE	:= $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))
#-----------------------------------------
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
#-----------------------------------------
.PHONY: clean, run

all: $(OUTPUT)
	
clean:
	rm -rf $(OUTPUT) $(OFILES)

run: $(OUTPUT)
	$(OUTPUT)

$(OUTPUT): $(OFILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OFILES) $(LIBS) 

$(OFILES): $(PRECOMPILED) 


$(PRECOMPILED) : src/precompiled.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $<

