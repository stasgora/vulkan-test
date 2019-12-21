#include <iostream>
#include "Window.h"

int main() {
	Window window;
	try {
		window.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
