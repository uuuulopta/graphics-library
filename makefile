TARGET = menu
SRCS = menu.cpp \
		 $(wildcard libraries/*.cpp) \
		 $(wildcard examples/*.cpp)
OBJDIR = obj

OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

COMMON_CFLAGS = -g -O0 -Wall 

ifeq ($(OS),Windows_NT)
	 CXX = clang++ -target x86_64-w64-mingw32
	 INCLUDE_PATHS = -IC:/MinGW/include -IC:/MinGW/include/c++
	 LIBRARY_PATHS = -LC:/MinGW/lib
	 LIBRARIES = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	 TARGET := $(TARGET).exe
	 MKDIR_P = mkdir $(subst /,\,$(@D)) >NUL 2>&1 || rem
	 RM_RF = rmdir /s /q $(subst /,\,$(OBJDIR)) >NUL 2>&1 || rem
	 RM_F = del $(subst /,\,$(TARGET)) >NUL 2>&1 || rem
else
	 CXX = clang++
	 INCLUDE_PATHS =
	 LIBRARY_PATHS =
	 LIBRARIES = -lSDL2main -lSDL2 -lSDL2_ttf
	 MKDIR_P = mkdir -p $(@D)
	 RM_RF = rm -rf $(OBJDIR)
	 RM_F = rm -f $(TARGET)
endif

all: $(TARGET)

$(OBJDIR)/%.o: %.cpp
	 $(MKDIR_P) 
	 $(CXX) $(COMMON_CFLAGS) $(INCLUDE_PATHS) -c $< -o $@

$(TARGET): $(OBJS)
	 $(CXX) $(OBJS) $(LIBRARY_PATHS) $(LIBRARIES) -o $@

clean:
	 -$(RM_RF)
	 -$(RM_F) 

.PHONY: all clean
