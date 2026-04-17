IMGUI_DIR ?= ../..

.SUFFIXES:

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

include $(DEVKITPRO)/wut/share/wut_rules


CXX += -std=c++20

CPPFLAGS := \
	-I$(IMGUI_DIR) \
	-I$(IMGUI_DIR)/backends \
	-I$(WUT_ROOT)/include \
	-DIMGUI_DISABLE_DEFAULT_SHELL_FUNCTIONS

CXXFLAGS := \
	-Wall -Wextra -Werror \
	-Os \
	-ffunction-sections \
	-fdata-sections \
	$(MACHDEP) \
	-D__WIIU__ \
	-D__wiiu__ \
	-D__WUT__

LDFLAGS := $(ARCH) $(RPXSPECS)

LIBS := \
	-L$(WUT_ROOT)/lib \
	-lwut


TARGET := example_wiiu_gx2

ELF_FILE := $(TARGET).elf

RPX_FILE := $(TARGET).rpx


SOURCES := \
	main.cpp \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_wiiu.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_gx2.cpp \


OBJECTS := $(addsuffix .o,$(basename $(notdir $(SOURCES))))


.PHONY: all clean run


all: $(RPX_FILE)


clean:
	$(RM) $(RPX_FILE)
	$(RM) $(ELF_FILE)
	$(RM) $(OBJECTS)


%.o: %.cpp
	$(CXX) -o $@ -c $< $(CPPFLAGS) $(CXXFLAGS)

%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) -o $@ -c $< $(CPPFLAGS) $(CXXFLAGS)

%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) -o $@ -c $< $(CPPFLAGS) $(CXXFLAGS)


$(ELF_FILE): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LIBS)


WIILOAD ?= tcp:wiiu
run: $(RPX_FILE)
	WIILOAD=$(WIILOAD) wiiload $<
