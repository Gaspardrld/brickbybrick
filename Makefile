# Compilateur et options de compilation
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = project

SRCS = project.cc game.cc message.cc brick.cc ball.cc paddle.cc tools.cc

# Fichiers objets correspondants
OBJS = $(SRCS:.cc=.o)

# Règle par défaut
all: $(TARGET)

# Édition de liens pour la création de l'exécutable final
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compilation séparée pour chaque fichier source
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Suppression des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)
