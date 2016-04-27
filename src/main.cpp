#include "lyst.h"
#include <fstream>
#include <string.h>

int main(int argc, char **argv) {{

    std::vector<int> pieces = {1000,1,4,2,5,2,2,1,4,4,2,1,9,6,5,0};
    int puzzleHeight = 3,
        //        puzzleWidth = 16;
        puzzleWidth = 6;

    /*
       std::vector<int> pieces = {0,1,0,0,1,1,2,0,3,0,1,0,0,2,1,0};
       int puzzleHeight = 3,
       puzzleWidth = 4;
       */
    /*
       std::vector<int> pieces = {0,0,0,0,2,0,0,0,2,0,0,0,0,2,2,0};
       int puzzleHeight = 2,
       puzzleWidth = 4;
       */
    /*
       std::vector<int> pieces = {0,1,0,1,0,2,3,1,3,1,2,0,1,1,0,0};
       int puzzleHeight = 4,
       puzzleWidth = 4;
       */

    Puzzle a (pieces, puzzleHeight, puzzleWidth);
    a.setLoading(false);
    // Command line arguments
    for (int i=0; i<argc; i++) {
        // Thread count
        if (!strncmp(argv[i],"--THREADS=",10)) {
            a.setMaxThreads(atoi(argv[i]+10));
        }
        // Max verbosity
        if (!strncmp(argv[i],"--VERBOSITY=",12)) {
            a.setVerbosity(atoi(argv[i]+12));
        }
        // Loading from last run
        if (!strncmp(argv[i],"--LOAD",6) || !strncmp(argv[i],"-l",2)) {
            a.setLoading( true );
        }
        // Help menu
        if (!strncmp(argv[i],"--HELP",6) || !strncmp(argv[i],"-h",2)) {
            printf("=================================\n");
            printf("||       Lyst help menu        ||\n");
            printf("=================================\n");
            printf(" ./Lyst [-h] [--THREADS] [--VERBOSITY] [-l|--LOAD]\n");
            printf("  --h              This help menu\n");
            printf("  --THREADS=n      Run solver with n threads\n");
            printf("  --VERBOSITY=n    Run solver with verbosity level n\n");
            printf("  --LOAD | -l      Run solver, continuing from log file\n");
            return 0;
        }
    }

    printf("Using %d threads... use './Lyst [threads] [verbosity]' to change\n",a.getMaxThreads());

    a.makeBlocks(a.isLoading());

    printf("Back in main\n");
    printf("Left: %lu\tMid: %lu\tRight: %lu\n",a.getLeftBlocks().size(), a.getMidBlocks().size(), a.getRightBlocks().size());

#ifdef USE_STRING_BLOCK
    std::vector<std::string> valid = a.getValidBlocks();
    printf("Found %lu valid solutions.\n",valid.size());
    if (a.isLoading()) {
        std::ofstream solutions("solutions.txt", std::ofstream::out | std::ofstream::app );
        for (unsigned int i=0; i<valid.size(); i++) {
            solutions << valid[i] << "\n";
        }
        solutions.close();
    } else {
        std::ofstream solutions("solutions.txt");
        for (unsigned int i=0; i<valid.size(); i++) {
            solutions << valid[i] << "\n";
        }
        solutions.close();
    }
    return 0;
#else
    //    std::vector< std::vector<uint16_t> > left = a.getLeftBlocks();
    //    for (int i=0; i<left.size(); i++) {
    //        std::cout << left[i] << "\n";
    //    }
    return 0;
#endif
}}
