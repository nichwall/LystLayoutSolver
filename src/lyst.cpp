#include "lyst.h"

Puzzle::Puzzle() {
    maxPieceCounts = {0};
    height = width = 0;
    max_threads = std::thread::hardware_concurrency();

    verbosity_level = 5;
}
Puzzle::Puzzle(std::vector<int> pieceCountData, int puzzleHeight, int puzzleWidth) {
    maxPieceCounts = pieceCountData;
    height = puzzleHeight;
    width = puzzleWidth;
    max_threads = std::thread::hardware_concurrency();

    verbosity_level = 5;
}

Puzzle::~Puzzle() {
}

// Helper functions used for generating things
int Puzzle::getPiece(std::string block, unsigned int index) {
    if (index < block.length())
        return (int)block[index]-96;
    return -1;
}
std::vector<int> Puzzle::getPieceCounts(std::string puzzle) {
    std::vector<int> count (height*width, 0);
    for (unsigned int i=0; i<puzzle.size()*height; i++) {
        int temp = getPiece(puzzle,i);
        count[temp]++;
    }
    return count;
}
// Piece count is valid, entire puzzle
bool Puzzle::pieceCountIsValid(std::string puzzle) {
    std::vector<int> count (16, 0);
    return pieceCountIsValid(puzzle, count);
}
// Piece count is valid, only new section of the puzzle
bool Puzzle::pieceCountIsValid(std::string newPart, std::vector<int> count) {
    for (unsigned int i=0; i<newPart.length(); i++) {
        int temp = getPiece(newPart,i);
        count[temp]++;
        if (count[temp] > maxPieceCounts[temp])
            return false;
    }
    return true;
}
bool Puzzle::checkAddition(std::string in, int inWidth, std::string added, int addedWidth) {
    for (unsigned int i=0; i<height; i++) {
        int leftPiece = getPiece(in, in.length()-3+i);
        int rightPiece = getPiece(added, i);
        if (pieceHasRight(leftPiece) != pieceHasLeft(rightPiece)) {
            return false;
        }
        if (leftPiece == 8 && rightPiece == 4)
            return false;
    }
    return pieceCountIsValid(in+added);
}
bool Puzzle::checkAddition(std::string in, int inWidth, std::string added, int addedWidth, std::vector<int> previousCounts) {
    for (unsigned int i=0; i<height; i++) {
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
    for (unsigned int i=0; i<puzzle.size()*height; i++) {
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
    for (unsigned int i=0; i<puzzle.size()*height; i++) {
        int temp = getPiece(puzzle,i);
        count[temp]++;
        if (count[temp] > maxPieceCounts[temp])
            return false;
    }
    return true;
}
bool Puzzle::checkAddition(std::vector<uint16_t> in, int inWidth, std::vector<uint16_t> added, int addedWidth) {
    for (unsigned int i=0; i<height; i++) {
        int leftPiece = getPiece(in, (i+1)*inWidth-1);
        int rightPiece = getPiece(added, i*addedWidth);
        if (pieceHasRight(leftPiece) != pieceHasLeft(rightPiece))
            return false;
        if (leftPiece == 8 && rightPiece == 4)
            return false;
    }
    // Appending the vector to in
    for (unsigned int i=0; i<added.size(); i++) {
        in.push_back(added[i]);
    }
    return pieceCountIsValid(in);
}
bool Puzzle::checkAddition(std::vector<uint16_t> in, int inWidth, std::vector<uint16_t> added, int addedWidth, std::vector<int> previousCounts) {
    for (unsigned int i=0; i<height; i++) {
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
        for (unsigned int i=0; i<height; i++) {
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
        // Breaks piece counts
        if ( !pieceCountIsValid(in+(char)(i+96)) )
            continue;

        bool broken = false;
        for (unsigned int j=0; j<in.length(); j++) {
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
    generateFirstSet();

    printf("Block counts: %lu\t%lu\t%lu\n",leftBlocks.size(),midBlocks.size(),rightBlocks.size());

    printf("Max threads: %u\n",max_threads);

#ifdef BREADTH_SEARCH
    leftWidth = 1;
    while (leftBlocks.size() != 0 && leftWidth != width) {
#endif // ifdef BREADTH_SEARCH
        // Start the threads
        std::vector<std::thread> threadPool;
        for (unsigned int i=0; i<max_threads; i++) {
            threadPool.push_back(std::thread(&Puzzle::combineLeftBlocks, this));
        }
        for (unsigned int i=0; i<max_threads; i++) {
            threadPool[i].join();
        }
#ifdef BREADTH_SEARCH
        mutex_left.lock();
        leftBlocks.swap(tempLeft);
        tempLeft.clear();
        if (leftBlocks.size() != 0) {
#ifdef USE_STRING_BLOCK
            leftWidth = leftBlocks[0].length()/height;
#else
            leftWidth = leftBlocks[0].size();
#endif // ifdef USE_STRING_BLOCK
        } else {
            leftWidth = 0;
        }
        printf("Left width: %lu\tBlocks: %lu\n",leftWidth,leftBlocks.size());
        mutex_left.unlock();
    }
#endif // ifdef BREADTH_SEARCH

    return;
}

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
        mutex_left.unlock();


        // Check whether we need to loop through all of the middle pieces or the right pieces
        // or if we're done
        bool useRight = false;
        if ( currentLeft.size()/height == width - 1 ) {
            useRight = true;
        } else if ( currentLeft.size()/height == width ) {
        // If total removed elements is less than the thread count after mod, save the blocks
            mutex_valid.lock();
            validSolutions.push_back(currentLeft);
            mutex_valid.unlock();
            continue;
        }

#ifdef USE_STRING_BLOCK
        // Check if we should output information about runtime
        if ( currentLeft.size()/height <= verbosity_level ) {
            printf("V-Level: %d\tLeft blocks: %lu\tSize: %lu\n",verbosity_level,leftBlocks.size(),currentLeft.size());
        }
#endif

        // If we're using the middle pieces
        if (!useRight) {
            for (unsigned int i=0; i<midBlocks.size(); i++) {
                // Check if we can add it
#ifdef USE_STRING_BLOCK
                if ( !checkAddition( currentLeft, currentLeft.length()/height, midBlocks[i], midBlocks[i].length()/height) )
#else
                if ( !checkAddition( currentLeft, currentLeft.size(), midBlocks[i], midBlocks[i].size()) )
#endif
                    continue;
                
                mutex_left.lock();
#ifdef USE_STRING_BLOCK
#ifdef BREADTH_SEARCH
                tempLeft.push_back( currentLeft+midBlocks[i] );
#else
                leftBlocks.push_back( currentLeft+midBlocks[i] );
#endif
#else
                std::vector<uint16_t> temp = currentLeft;
                for (unsigned int j=0; j<midBlocks[i].size(); j++) {
                    temp.push_back(midBlocks[i][j]);
                }
#ifdef BREADTH_SEARCH
                tempLeft.push_back( temp );
#else
                leftBlocks.push_back( temp );
#endif
#endif
                mutex_left.unlock();
            }
        } else {
            for (unsigned int i=0; i<rightBlocks.size(); i++) {
                // Check if we can add it
#ifdef USE_STRING_BLOCK
                if ( !checkAddition( currentLeft, currentLeft.length(), rightBlocks[i], rightBlocks[i].length()) )
#else
                if ( !checkAddition( currentLeft, currentLeft.size(), rightBlocks[i], rightBlocks[i].size()) )
#endif
                    continue;

                mutex_left.lock();
#ifdef USE_STRING_BLOCK
#ifdef BREADTH_SEARCH
                tempLeft.push_back( currentLeft+rightBlocks[i] );
#else
                leftBlocks.push_back( currentLeft+rightBlocks[i] );
#endif // ifdef BREADTH_SEARCH
#else  // ifdef USE_STRING_BLOCK
                std::vector<uint16_t> temp = currentLeft;
                for (unsigned int j=0; j<rightBlocks[i].size(); j++) {
                    temp.push_back(rightBlocks[i][j]);
                }
#ifdef BREADTH_SEARCH
                tempLeft.push_back( temp );
#else // ifdef BREADTH_SEARCH
                leftBlocks.push_back( temp );
#endif // ifdef BREADTH_SEARCH
#endif // ifdef USE_STRING_BLOCK
                mutex_left.unlock();
            }
        }
    }
}

#ifndef USE_STRING_BLOCK
uint16_t Puzzle::stou (std::string block) {
    // Converting from std::string to short.
    uint16_t newBlock = 0;
    for (unsigned int i=0; i<height; i++) {
        newBlock += ((int)block.at(i)-96) * pow(16,i);
    }
    return newBlock;
}
std::string Puzzle::utos (uint16_t block) {
    std::vector<uint16_t> vect = { block };
    // Converting from uint16_t to std::string
    std::string newBlock = "";
    for (unsigned int i=0; i<height; i++) {
        char temp = 96;
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
