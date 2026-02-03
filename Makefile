BUILD_DIR = build
OUTPUT_DIR = $(BUILD_DIR)/output
INSTALL_DIR = $(BUILD_DIR)/install

CLANG_BUILD_DIR = build_clang
CLANG_OUTPUT_DIR = $(CLANG_BUILD_DIR)/output


.PHONY: clean build

build:
	@echo "Starting build process... $(shell nproc) cores"
	cmake -B $(BUILD_DIR) -DDATABASE_ARMORY_BUILD_TESTS=ON -DDATABASE_ARMORY_BUILD_EXAMPLE=ON
	cmake --build $(BUILD_DIR) -j$(shell nproc)
	# cp $(BUILD_DIR)/example/database_armory_example $(OUTPUT_DIR)

build-clang:
	@echo "Starting Clang build process... $(shell nproc) cores"
	cmake -B $(CLANG_BUILD_DIR) -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DDATABASE_ARMORY_BUILD_TESTS=ON -DDATABASE_ARMORY_BUILD_EXAMPLE=ON -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(CLANG_BUILD_DIR) -j$(shell nproc)
	mkdir -p $(CLANG_OUTPUT_DIR)
	# cp $(CLANG_BUILD_DIR)/example/database_armory_example $(CLANG_OUTPUT_DIR)


clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)
	rm -rf $(OUTPUT_DIR)
	mkdir -p $(OUTPUT_DIR)

rebuild: clean build
