#define _CRT_SECURE_NO_WARNINGS // Disable warnings about unsafe functions like getenv
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string commandInput, echoInput;

int main() {

    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    const char path_delimiter = ';';

    do {
        std::cout << "$ ";
        std::getline(std::cin, commandInput);

        if (commandInput == "exit") {
            break;
        }
        // echo prints the string to the console
        else if (commandInput.substr(0, 4) == "echo") {
            commandInput.erase(0, 5);
            std::cout << commandInput << std::endl;
        }
		// type checks if the command is a shell builtin or an executable in the PATH
        else if (commandInput.substr(0, 4) == "type") {
            commandInput.erase(0, 5);

            if (commandInput == "exit" || commandInput == "echo" || commandInput == "type") {
                std::cout << commandInput << " is a shell builtin" << std::endl;
            }
            else {
                const char* pathEnv = std::getenv("PATH");
                bool found = false;

                if (pathEnv != nullptr) {
                    std::stringstream ss(pathEnv);
                    std::string dir;

                    while (std::getline(ss, dir, path_delimiter)) {

                        fs::path base_path = fs::path(dir) / commandInput;
                        fs::path exe_path = base_path.string() + ".exe";

                        if (fs::exists(base_path) && fs::is_regular_file(base_path)) {
                            std::cout << commandInput << " is " << base_path.string() << std::endl;
                            found = true;
                            break;
                        }
                        else if (fs::exists(exe_path) && fs::is_regular_file(exe_path)) {
                            std::cout << commandInput << " is " << exe_path.string() << std::endl;
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        std::cout << commandInput << ": not found" << std::endl;
                    }
                }
            }
        }
        else {
            std::cout << commandInput << ": command not found" << std::endl;
        }

    } while (true);

    return 0;
}