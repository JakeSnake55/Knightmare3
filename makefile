#Don't mess with this
SOURCES := $(wildcard source/*.cpp) $(wildcard source/IMGUI/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

#Name of this file
TITLE = makefile

#Executable file name
OUTPUT = km

ARGS=

# WARNINGS for g++
WARNING := -Wall -Wextra
LINKFLAGS := $$( pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_main-5 allegro_image-5 allegro_color-5 --libs --cflags)


# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all run clean deepclean install

# The first rule is the default, ie. "make",
# "make all" mean the same
all: $(OUTPUT)

run: 
	@./$(OUTPUT) $(ARGS)

clean:
	@$(RM) $(OBJECTS) $(DEPENDS)

deepclean:
	@$(RM) $(OBJECTS) $(DEPENDS) $(OUTPUT)

# Linking the executable from the object files
$(OUTPUT): $(OBJECTS)
	@echo
	@$(CXX) $(WARNING) $(CXXFLAGS) $^ -o $@ $(LINKFLAGS)
	@echo "Linked "$(OBJECTS) "Into "$(OUTPUT)

-include $(DEPENDS)

%.o: %.cpp $(TITLE)
	@$(CXX) $(WARNING) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "Compiled "$< "to "$(patsubst %.cpp,%.o,$<)



