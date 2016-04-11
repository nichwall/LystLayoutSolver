#include "lyst.h"
#include <fstream>

int main(int argc, char **argv) {{

    std::vector<int> pieces = {1000,1,4,2,5,2,2,1,4,4,2,1,9,6,5,0};
    int puzzleHeight = 3,
//        puzzleWidth = 16;
        puzzleWidth = 4;

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
    if (argc == 2) {
        a.setMaxThreads(atoi(argv[1]));
    } else if (argc == 3) {
        a.setMaxThreads(atoi(argv[1]));
        a.setVerbosity(atoi(argv[2]));
    }

/*    // Printing piece counts
    std::vector<int> counts = a.getMaxPieceCount();
    for (int i=0; i<counts.size(); i++) {
        std::cout << counts[i] << " ";
    }printf("\n");
    */
    // Printing mid blocks

    printf("Using %d threads... use './Lyst [threads] [verbosity]' to change\n",a.getMaxThreads());

    a.makeBlocks();
    
    printf("Back in main\n");
    printf("Left: %lu\tMid: %lu\tRight: %lu\n",a.getLeftBlocks().size(), a.getMidBlocks().size(), a.getRightBlocks().size());

#ifdef USE_STRING_BLOCK
    std::vector<std::string> valid = a.getValidBlocks();
    printf("Found %lu valid solutions.\n",valid.size());
    std::ofstream solutions("solutions.txt");
    for (unsigned int i=0; i<valid.size(); i++) {
        solutions << valid[i] << "\n";
    }
    solutions.close();
    return 0;
#else
//    std::vector< std::vector<uint16_t> > left = a.getLeftBlocks();
//    for (int i=0; i<left.size(); i++) {
//        std::cout << left[i] << "\n";
//    }
    return 0;
#endif
}}
