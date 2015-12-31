#include "lyst.h"

int main(int argc, char **argv) {
    std::vector<int> pieces = {1000,1,4,2,5,2,2,1,4,4,2,1,4,4,2,1,9,6,5,0};
    int puzzleHeight = 3,
        puzzleWidth = 16;
    Puzzle a (pieces, puzzleHeight, puzzleWidth);
    if (argc == 2) {
        a.setMaxThreads(atoi(argv[1]));
    } else {
        printf("Using %d threads... use './Lyst [threads]' to change\n",a.getMaxThreads());
    }

    a.makeBlocks();
    
    printf("Left: %d\tMid: %d\tRight: %d\n",a.getLeftBlocks().size(), a.getMidBlocks().size(), a.getRightBlocks().size());

//    std::vector< std::vector<uint16_t> > left = a.getLeftBlocks();
//    for (int i=0; i<left.size(); i++) {
//        std::cout << left[i] << "\n";
//    }
}
