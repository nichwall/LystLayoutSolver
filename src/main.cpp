#include "lyst.h"

int main(int argc, char **argv) {
/*
    std::vector<int> pieces = {1000,1,4,2,5,2,2,1,4,4,2,1,9,6,5,0};
    int puzzleHeight = 3,
        puzzleWidth = 16;
*/
    std::vector<int> pieces = {1000,1,0,0,1,1,2,0,3,0,1,0,0,2,1,0};
    int puzzleHeight = 3,
        puzzleWidth = 4;

    for (int i=1; i<pieces.size(); i++) {
        std::cout << (char)(96+i) << ": " << pieces[i] << "\n";
    }

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
    
    printf("Left: %d\tMid: %d\tRight: %d\n",a.getLeftBlocks().size(), a.getMidBlocks().size(), a.getRightBlocks().size());

#ifdef USE_STRING_BLOCK
    std::vector<std::string> valid = a.getValidBlocks();
    printf("Found %d valid solutions.\n",valid.size());
    for (int i=0; i<valid.size(); i++) {
        std::cout << valid[i] << "\n";
    }
    return 0;
#else
//    std::vector< std::vector<uint16_t> > left = a.getLeftBlocks();
//    for (int i=0; i<left.size(); i++) {
//        std::cout << left[i] << "\n";
//    }
    return 0;
#endif
}
