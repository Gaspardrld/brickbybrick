CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS := -MMD -MP
LDFLAGS  :=
LDLIBS   :=

TARGET := project

# Adapte ces dossiers à TON arborescence réelle.
# Rendu 1 : pas de gui/graphic/gtkmm.
SRC_DIRS := . tools message game brick ball paddle project

# Récupère tous les .cc/.cpp des dossiers listés (ceux qui existent)
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.cc) $(wildcard $(d)/*.cpp)) \
        $(wildcard *.cc) $(wildcard *.cpp)

# Si tu veux forcer l'exclusion de modules GTK/mmême s’ils traînent :
SRCS := $(filter-out %/gui.cc %/gui.cpp %/graphic.cc %/graphic.cpp,$(SRCS))

OBJS := $(SRCS:.cc=.o)
OBJS := $(OBJS:.cpp=.o)

DEPS := $(OBJS:.o=.d)

.PHONY: all clean distclean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Règles de compilation (pour .cc et .cpp)
%.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS)

distclean: clean
	rm -f $(TARGET)

# Usage: make run TEST=t01.txt
run: $(TARGET)
	./$(TARGET) $(TEST)
