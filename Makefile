BUILD_DIR = build
OUTPUT_DIR = $(BUILD_DIR)/output


.PHONY: clean build

build:
	@echo "Starting build process... $(shell nproc) cores"
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) -j$(shell nproc)
	cp $(BUILD_DIR)/example/db_example $(OUTPUT_DIR)
	cp $(BUILD_DIR)/src/libdatabase.a $(OUTPUT_DIR)


clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)
	rm -rf $(OUTPUT_DIR)
	mkdir -p $(OUTPUT_DIR)


rebuild: clean build
