
CXX	:= g++
LD	:= g++

CXXFLAGS	:= -std=c++11 -Wall -Wextra -pedantic -fno-rtti -fno-exceptions
LDFLAGS		:=
LIBS		:=

ifeq ($(OS),Windows_NT)
	TARGET	:= $(shell basename $(CURDIR)).exe
else
	TARGET	:= $(shell basename $(CURDIR))
endif

OUTPUT		:= $(CURDIR)/$(TARGET)
BUILD		:= build
SOURCES		:= src
INCLUDES	:= $(BUILD)
INCLUDE		:= $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

CXXFILES	:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
OFILES		:= $(CXXFILES:%.cpp=$(BUILD)/%.o)
PRECOMPILED	:= $(BUILD)/precompiled.hpp.gch
DEPFILES	:= $(OFILES:.o=.d)

export VPATH	:= $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
#-----------------------------------------
$(BUILD)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -MMD $(INCLUDE) -c $< -o $@
#-----------------------------------------
.PHONY: all clean debug release run

all: release

debug: CXXFLAGS	:= $(CXXFLAGS) -g -Og -D_DEBUG
debug: LDFLAGS	:= -g
debug: $(OUTPUT)

release: CXXFLAGS	:= $(CXXFLAGS) -O3 -fomit-frame-pointer
release: LDFLAGS	:= -s
release: $(OUTPUT)

$(BUILD):
	@mkdir -p $@

$(OUTPUT): $(BUILD) $(OFILES)
	$(CXX) $(LDFLAGS) -o $@ $(OFILES) $(LIBS)

$(OFILES): $(PRECOMPILED)

$(PRECOMPILED) : src/precompiled.hpp
	$(CXX) $(CXXFLAGS) -MMD $(INCLUDE) -o $@ $<

clean:
	rm -rf $(OUTPUT) $(BUILD)

run: $(OUTPUT)
	$(OUTPUT)

-include $(DEPFILES)

