# Makefile per il progetto Caserma

# Compilatore e flag di compilazione
CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -Iinclude
LDFLAGS := 

# Directory
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin

# File sorgente e oggetti
SOURCES := main.cpp $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:.cpp=.o)))
TARGET := $(BIN_DIR)/caserma

# Target di default
all: $(TARGET)

# Creazione della directory build se non esiste
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Creazione della directory bin se non esiste
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilazione dell'eseguibile
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✓ Compilazione completata: $(TARGET)"

# Compilazione dei file oggetto
$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target di pulizia
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "✓ Pulizia completata"

# Target di esecuzione
run: $(TARGET)
	./$(TARGET)

# Target di rebuild
rebuild: clean all

# Target di help
help:
	@echo "Target disponibili:"
	@echo "  make              - Compila il progetto"
	@echo "  make run          - Compila ed esegue il progetto"
	@echo "  make clean        - Rimuove i file compilati"
	@echo "  make rebuild      - Ricompila da zero"
	@echo "  make help         - Mostra questo messaggio"

# Phony target
.PHONY: all clean run rebuild help
