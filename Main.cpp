#include "Triangle.hpp"

int main(int argc, char* argv[])
{
	Triangle t;

	try
	{
		t.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;

		return EXIT_FAILURE;
	}

	return 0;
}