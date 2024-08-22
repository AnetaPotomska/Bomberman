#include <exception>

#include "Application.h"
#include "ConfigurationManager.h"

#define CONFIG_PATH "./examples/config.json"

int main(int argc, char *argv[]) {
	ConfigurationManager conf;
	try {
		conf.setupConfig(CONFIG_PATH);
		Application app(conf);
		app.run();
	}
	catch(const char * err) {
        printf("Error: %s\n", err);
        return 1;
	}
	catch(const std::exception & err) {
        printf("Error: %s\n", err.what());
        return 1;
	}
	return 0;
}