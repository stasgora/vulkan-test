#include <iostream>
#include "Window.h"

using namespace std;

int main() {
	Window window;
	try {
		window.run();
	} catch (const exception& e) {
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
