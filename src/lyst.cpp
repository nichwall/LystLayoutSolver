#include "lyst.h"

Puzzle::Puzzle() {
    maxPieceCounts = {0,0,0,0,0,0,0,0};
    height = width = 0;
    max_threads = std::thread::hardware_concurrency();

    leftBlockFileName  = "leftBlocks.txt";
    midBlockFileName   = "midBlocks.txt";
    rightBlockFileName = "rightBlocks.txt";
    validFileName      = "validBlocks.txt";
}
Puzzle::Puzzle(std::vector<int> pieceCountData, int puzzleHeight, int puzzleWidth) {
    maxPieceCounts = pieceCountData;
    height = puzzleHeight;
    width = puzzleWidth;
    max_threads = std::thread::hardware_concurrency();

    leftBlockFileName  = "leftBlocks.txt";
    midBlockFileName   = "midBlocks.txt";
    rightBlockFileName = "rightBlocks.txt";
    validFileName      = "validBlocks.txt";
}

Puzzle::~Puzzle() {

}

// Helper functions used for generating things
int Puzzle::getPiece(std::string block, int index) {
    if (index < block.length())
        return (int)block[index]-96;
    return 0;
}
std::vector<int> Puzzle::getPieceCounts(std::string puzzle) {
    std::vector<int> count (height*width, 0);
    for (int i=0; i<puzzle.size()*height; i++) {
        int temp = getPiece(puzzle,i);
        count[temp]++;
    }
    return count;
}
bool Puzzle::pieceCountIsValid(std::string puzzle) {
    std::vector<int> count (height*width, 0);
    return pieceCountIsValid(puzzle, count);
}
bool Puzzle::pieceCountIsValid(std::string puzzle, std::vector<int> count) {
    for (int i=0; i<puzzle.length()*height; i++) {
        int temp = getPiece(puzzle,i);
        count[temp]++;
        if (count[temp] > maxPieceCounts[temp])
            return false;
    }
    return true;
}
bool Puzzle::checkAddition(std::string in, int inWidth, std::string added, int addedWidth) {
    for (int i=0; i<height; i++) {
        int leftPiece = getPiece(in, (i+1)*inWidth-1);
        int rightPiece = getPiece(added, i*addedWidth);
        if (pieceHasRight(leftPiece) != pieceHasLeft(rightPiece))
            return false;
        if (leftPiece == 8 && rightPiece == 4)
            return false;
    }
    return pieceCountIsValid(in+added);
}
bool Puzzle::checkAddition(std::string in, int inWidth, std::string added, int addedWidth, std::vector<int> previousCounts) {
    for (int i=0; i<height; i++) {
        int leftPiece = getPiece(in, (i+1)*inWidth-1);
        int rightPiece = getPiece(added, i*addedWidth);
        if (pieceHasRight(leftPiece) != pieceHasLeft(rightPiece))
            return false;
        if (leftPiece == 8 && rightPiece == 4)
            return false;
    }
    return pieceCountIsValid(added,previousCounts);
}

#ifndef USE_STRING_BLOCK
int Puzzle::getPiece(std::vector<uint16_t> block, int index) {
    if ( index < block.size()*height ) {
        // Getting the column out of the vector
        uint16_t tempVal = block[index/height];
        // Getting the piece from the column
        return (tempVal / (int)pow(16,index%height) ) % 16;
    }
    return 0;
}
std::vector<int> Puzzle::getPieceCounts(std::vector<uint16_t> puzzle) {
    std::vector<int> count (height*width, 0);
    for (int i=0; i<puzzle.size()*height; i++) {
        int temp = getPiece(puzzle,i);
        count[temp]++;
    }
    return count;
}
bool Puzzle::pieceCountIsValid(std::vector<uint16_t> puzzle) {
    std::vector<int> count (height*width, 0);
    return pieceCountIsValid(puzzle, count);
}
bool Puzzle::pieceCountIsValid(std::vector<uint16_t> puzzle, std::vector<int> count) {
    for (int i=0; i<puzzle.size()*height; i++) {
        int temp = getPiece(puzzle,i);
        count[temp]++;
        if (count[temp] > maxPieceCounts[temp])
            return false;
    }
    return true;
}
bool Puzzle::checkAddition(std::vector<uint16_t> in, int inWidth, std::vector<uint16_t> added, int addedWidth) {
    for (int i=0; i<height; i++) {
        int leftPiece = getPiece(in, (i+1)*inWidth-1);
        int rightPiece = getPiece(added, i*addedWidth);
        if (pieceHasRight(leftPiece) != pieceHasLeft(rightPiece))
            return false;
        if (leftPiece == 8 && rightPiece == 4)
            return false;
    }
    // Appending the vector to in
    for (int i=0; i<added.size(); i++) {
        in.push_back(added[i]);
    }
    return pieceCountIsValid(in);
}
bool Puzzle::checkAddition(std::vector<uint16_t> in, int inWidth, std::vector<uint16_t> added, int addedWidth, std::vector<int> previousCounts) {
    for (int i=0; i<height; i++) {
        int leftPiece = getPiece(in, (i+1)*inWidth-1);
        int rightPiece = getPiece(added, i*addedWidth);
        if (pieceHasRight(leftPiece) != pieceHasLeft(rightPiece))
            return false;
        if (leftPiece == 8 && rightPiece == 4)
            return false;
    }
    return pieceCountIsValid(added,previousCounts);
}
#endif // ifndef USE_STRING_BLOCK

void Puzzle::generateFirstSet() {
    generateFirstSet("");
}
void Puzzle::generateFirstSet(std::string in) {
    // Check if we're done adding to the current block
    if (in.length() >= height) {
        // Check that the bottom is still valid
        if (pieceHasBottom( getPiece(in, height-1) ))
            return;

        bool r = false,
             l = false;
        for (int i=0; i<height; i++) {
            int piece = getPiece(in, i);
            if (pieceHasRight(piece))
                r = true;
            if (pieceHasLeft(piece))
                l = true;
        }
        // If the piece doesn't have a right or a left, then it's bad
        if (!r && !l)
            return;

#ifdef USE_STRING_BLOCK
        std::string toAdd = in;
#else
        std::vector<uint16_t> toAdd = { stou(in) };
#endif // ifdef USE_STRING_BLOCK

        // Is a right block?
        if (!r && l) {
            rightBlocks.push_back(toAdd);
            return;
        }
        // Left block?
        if (r && !l) {
            leftBlocks.push_back(toAdd);
            return;
        }
        // Otherwise, it's a mid block
        midBlocks.push_back(toAdd);
        return;
    }

    for (int i=1; i<15; i++) {
        // Matches above state
        int abovePiece = 0;
        if (in.length() != 0) {
            abovePiece = getPiece( in, in.length()-1 );
        }
        if (pieceHasBottom(abovePiece) != pieceHasTop(i))
            continue;

        // Check that A is not below B
        if (i==1 && abovePiece==2)
            continue;
        // Check if we have too many of a piece
        std::vector<int> counts (16,0);
        bool broken = false;
        for (int j=0; j<in.length(); j++) {
            int temp = getPiece(in,j);
            counts[temp]++;
            if (counts[temp] > maxPieceCounts[temp]) {
                broken = true;
                break;
            }
        }

        if (broken)
            continue;

        // No errors occurred, recurse
        generateFirstSet( in + (char)(i+96) );
    }
    return;
}

// Generate all of the blocks of desired width
void Puzzle::makeBlocks() {
#ifdef USE_STRING_BLOCK
    // Testing CMake things
    // Try to load blocks from a previous run
    loadBlocks();
    // If nothing was loaded, then we need to generate the initial pieces
    if (midBlocks.size() == 0 || rightBlocks.size() == 0)
#endif // ifdef USE_STRING_BLOCK
        generateFirstSet();

    printf("Block counts: %d\t%d\t%d\n",leftBlocks.size(),midBlocks.size(),rightBlocks.size());

    printf("Max threads: %d\n",max_threads);

#ifdef BREADTH_SERACH
    while (leftBlocks().size() != 0 && leftWidth != width) {
#endif
        // Start the threads
        std::vector<std::thread> threadPool;
        for (int i=0; i<max_threads; i++) {
            threadPool.push_back(std::thread(&Puzzle::combineLeftBlocks, this));
        }
        for (int i=0; i<max_threads; i++) {
            threadPool[i].join();
        }
#ifdef BREADTH_SEARCH
        leftBlocks.swap(tempLeft);
        tempLeft.clear();
        if (leftBlocks.size() != 0) {
#ifdef USE_STRING_BLOCK
            leftWidth = leftBlocks[0].length()/height;
#else
            leftWidth = leftBlocks[0].size();
#endif
        } else {
            leftWidth = 0;
        }
    }
#endif

    return;
}

unsigned long long totalRemoved = 0;

void Puzzle::combineLeftBlocks() {
    // We need to loop for as long as there's items left in the vector
    while (true) {
        // Get the first item, then remove so that others don't use it
        mutex_left.lock();
        if (leftBlocks.size() == 0) {
            mutex_left.unlock();
            break;
        }
#ifdef USE_STRING_BLOCK
        std::string currentLeft = leftBlocks.back();
#else
        std::vector<uint16_t> currentLeft = leftBlocks.back();
#endif
        leftBlocks.pop_back();
        totalRemoved++;
        // If total removed elements is less than the thread count after mod, save the blocks
#ifdef USE_STRING_BLOCK
#ifdef STORE_COMP
        if (totalRemoved % 30000 == 0) {
            mutex_savingBlocks.lock();
            saveBlocks();
            mutex_savingBlocks.unlock();
        }
#endif
#endif
        mutex_left.unlock();


        // Check whether we need to loop through all of the middle pieces or the right pieces
        // or if we're done
        bool useRight = false;
        if ( currentLeft.size() == width - 1 ) {
            useRight = true;
        } else if ( currentLeft.size() == width ) {
        // If total removed elements is less than the thread count after mod, save the blocks
            mutex_valid.lock();
            validSolutions.push_back(currentLeft);
            mutex_valid.unlock();
            continue;
        }

        // If we're using the middle pieces
        if (!useRight) {
            for (int i=0; i<midBlocks.size(); i++) {
                // Check if we can add it
#ifdef USE_STRING_BLOCK
                if ( !checkAddition( currentLeft, currentLeft.length(), midBlocks[i], midBlocks[i].length()) )
#else
                if ( !checkAddition( currentLeft, currentLeft.size(), midBlocks[i], midBlocks[i].size()) )
#endif
                    continue;
                
                mutex_left.lock();
#ifdef STORE_COMP
                mutex_savingBlocks.lock_shared();
#endif
#ifdef USE_STRING_BLOCK
#ifdef BREADTH_SEARCH
                tempLeft.push_back( currentLeft+midBlocks[i] );
#else
                leftBlocks.push_back( currentLeft+midBlocks[i] );
#endif
#else
                std::vector<uint16_t> temp = currentLeft;
                for (int j=0; j<midBlocks[i].size(); j++) {
                    temp.push_back(midBlocks[i][j]);
                }
#ifdef BREADTH_SEARCH
                tempLeft.push_back( temp );
#else
                leftBlocks.push_back( temp );
#endif
#endif
#ifdef STORE_COMP
                mutex_savingBlocks.unlock_shared();
#endif
                mutex_left.unlock();
            }
        } else {
            for (int i=0; i<rightBlocks.size(); i++) {
                // Check if we can add it
#ifdef USE_STRING_BLOCK
                if ( !checkAddition( currentLeft, currentLeft.length(), rightBlocks[i], rightBlocks[i].length()) )
#else
                if ( !checkAddition( currentLeft, currentLeft.size(), rightBlocks[i], rightBlocks[i].size()) )
#endif
                    continue;

                mutex_left.lock();
#ifdef STORE_COMP
                mutex_savingBlocks.lock_shared();
#endif
#ifdef USE_STRING_BLOCK
#ifdef BREADTH_SEARCH
                tempLeft.push_back( currentLeft+midBlocks[i] );
#else
                leftBlocks.push_back( currentLeft+midBlocks[i] );
#endif
#else
                std::vector<uint16_t> temp = currentLeft;
                for (int j=0; j<rightBlocks[i].size(); j++) {
                    temp.push_back(rightBlocks[i][j]);
                }
#ifdef BREADTH_SEARCH
                tempLeft.push_back( temp );
#else
                leftBlocks.push_back( temp );
#endif
#endif
#ifdef STORE_COMP
                mutex_savingBlocks.unlock_shared();
#endif
                mutex_left.unlock();
            }
        }
    }
}

#ifdef USE_STRING_BLOCK
// Block saving functions
void Puzzle::saveBlocks() {
    // Write left blocks
    std::ofstream leftBlockFile (leftBlockFileName.c_str());
    if (leftBlockFile.good()) {
        for (int i=0; i<leftBlocks.size(); i++) {
            leftBlockFile << leftBlocks[i] << "\n";
        }
        leftBlockFile.close();
    }
    // Write mid blocks
    std::ofstream midBlockFile (midBlockFileName.c_str());
    if (midBlockFile.good()) {
        for (int i=0; i<midBlocks.size(); i++) {
            midBlockFile << midBlocks[i] << "\n";
        }
        midBlockFile.close();
    }
    // Write right blocks
    std::ofstream rightBlockFile (rightBlockFileName.c_str());
    if (rightBlockFile.good()) {
        for (int i=0; i<rightBlocks.size(); i++) {
            rightBlockFile << rightBlocks.size() << "\n";
        }
        rightBlockFile.close();
    }
    // Write blocks that are already long enough
    std::ofstream validFile (validFileName.c_str());
    if (validFile.good()) {
        for (int i=0; i<validSolutions.size(); i++) {
            validFile << validSolutions[i] << "\n";
        }
        validFile.close();
    }
}
// Block loading functions
void Puzzle::loadBlocks() {
    std::string line;
    // Warning: will replace old vectors if there is a save file
    std::ifstream leftBlockFile (leftBlockFileName.c_str());
    if (leftBlockFile.good()) {
        leftBlocks.clear();
        while ( getline(leftBlockFile, line) ) {
            leftBlocks.push_back(line);
        }
        leftBlockFile.close();
    }
    std::ifstream midBlockFile (midBlockFileName.c_str());
    if (midBlockFile.good()) {
        midBlocks.clear();
        while ( getline(midBlockFile, line) ) {
            midBlocks.push_back(line);
        }
        midBlockFile.close();
    }
    std::ifstream rightBlockFile (rightBlockFileName.c_str());
    if (rightBlockFile.good()) {
        rightBlocks.clear();
        while ( getline(rightBlockFile, line) ) {
            rightBlocks.push_back(line);
        }
        rightBlockFile.close();
    }
    std::ifstream validFile (validFileName.c_str());
    if (validFile.good()) {
        validSolutions.clear();
        while ( getline(validFile, line) ) {
            validSolutions.push_back(line);
        }
        validFile.close();
    }
}
#endif

#ifndef USE_STRING_BLOCK
uint16_t Puzzle::stou (std::string block) {
    // Converting from std::string to short.
    uint16_t newBlock = 0;
    for (int i=0; i<height; i++) {
        newBlock += ((int)block.at(i)-96) * pow(16,i);
    }
    return newBlock;
}
std::string Puzzle::utos (uint16_t block) {
    std::vector<uint16_t> vect = { block };
    // Converting from uint16_t to std::string
    std::string newBlock = "";
    for (int i=0; i<height; i++) {
        char temp = (char)('a'-1);
        // Adding to the block
        if ( pieceHasTop( getPiece(vect,i) ) )
            temp += 1;
        if ( pieceHasBottom( getPiece(vect,i) ) )
            temp += 2;
        if ( pieceHasLeft( getPiece(vect,i) ) )
            temp += 4;
        if ( pieceHasRight( getPiece(vect,i) ) )
            temp += 8;
        newBlock += temp;
    }
    return newBlock;
}
#endif
