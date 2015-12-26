#include "lyst.h"

int main() {
    std::vector<int> pieces = {1000,4,5,8,4,2,2,5,1,5,3,5,2,1,1,0};
    int puzzleHeight = 3,
        puzzleWidth = 16,
        desiredWidth = 8;
    Puzzle a (pieces, puzzleHeight, puzzleWidth, desiredWidth);

    printf("Left: %d\tMid: %d\tRight: %d\n",a.getLeftBlocks().size(), a.getMidBlocks().size(), a.getRightBlocks().size());

    a.makeBlocks();
    
    printf("Left: %d\tMid: %d\tRight: %d\n",a.getLeftBlocks().size(), a.getMidBlocks().size(), a.getRightBlocks().size());

    std::cout << "Desired Left: " << a.getDesiredLeftWidth() << "\n";
    std::cout << "Desired Mid: " << a.getDesiredMidWidth() << "\n";
    std::cout << "Desired Right: " << a.getDesiredRightWidth() << "\n";
/*    std::vector<std::string> left = a.getLeftBlocks(),
        mid = a.getMidBlocks();
    for (int i=0; i<left.size(); i++) {
        for (int j=0; j<left[i].length(); j++) {
            std::cout << (int)left[i].at(j) << "\t";
        }
        std::cout << "\n";
//        std::cout << left[i] << "\n";
    }
    std::cout << "\n\n\n";
    for (int i=0; i<mid.size(); i++) {
        for (int j=0; j<mid[i].length(); j++) {
            std::cout << (int)mid[i].at(j) << "\t";
        }
        std::cout << "\n";
//        std::cout << mid[i] << "\n";
    }
*/    
}
