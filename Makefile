# Nome do executável
TARGET = main.exe

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Arquivos-fonte e objetos
SRCS = main.cpp GrafoLista.cpp
OBJS = $(SRCS:.cpp=.o)

# Regra padrão
all: $(TARGET)

# Como compilar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Como compilar arquivos .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos compilados
clean:
	del /Q *.o *.exe 2> NUL || true

# Força a recompilação
rebuild: clean all
