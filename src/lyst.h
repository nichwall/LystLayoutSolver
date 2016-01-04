#ifndef _SRC_LYST_H_
#define _SRC_LYST_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>
#include <math.h>

#include "lystConfig.h"

// Don't use shared_mutex if we don't need it. Travis doesn't like it
#ifdef STORE_COMP
#include <shared_mutex>
#endif

class Puzzle {
    public:
        // Constructors
        Puzzle();
        Puzzle(std::vector<int> pieceCountData, int puzzleHeight, int puzzleWidth);

        // Destructors
        ~Puzzle();

        // Accessors
        int getPuzzleHeight() { return height;      }
        int getPuzzleWidth()  { return width;       }
        int getMaxThreads()   { return max_threads; }

        void setMaxThreads(int threadCount) { if (threadCount > 0) max_threads = threadCount; }
        
#ifdef USE_STRING_BLOCK
        std::vector<std::string> getLeftBlocks()  { return leftBlocks;  }
        std::vector<std::string> getMidBlocks()   { return midBlocks;   }
        std::vector<std::string> getRightBlocks() { return rightBlocks; }
        std::string getLeftBlocks(int index)      { return leftBlocks[index];  }
        std::string getMidBlocks(int index)       { return midBlocks[index];   }
        std::string getRightBlocks(int index)     { return rightBlocks[index]; }
#else
        std::vector< std::vector<uint16_t> > getLeftBlocks()  { return leftBlocks;  }
        std::vector< std::vector<uint16_t> > getMidBlocks()   { return midBlocks;   }
        std::vector< std::vector<uint16_t> > getRightBlocks() { return rightBlocks; }
        std::vector<uint16_t> getLeftBlocks(int index)        { return leftBlocks[index];  }
        std::vector<uint16_t> getMidBlocks(int index)         { return midBlocks[index];   }
        std::vector<uint16_t> getRightBlocks(int index)       { return rightBlocks[index]; }
#endif

        // Block creations
        void makeBlocks();

    private:
        // Functions related to block generation
        void generateFirstSet();
        void generateFirstSet(std::string in);

        void combineLeftBlocks();

        // Helper functions in generating
        int getPiece(std::string block, int index);
        std::vector<int> getPieceCounts(std::string puzzle);
        bool pieceCountIsValid(std::string puzzle);
        bool pieceCountIsValid(std::string puzzle, std::vector<int> count);
        bool checkAddition(std::string in, int inWidth, std::string added, int addedWidth);
        bool checkAddition(std::string in, int inWidth, std::string added, int addedWidth, std::vector<int> previousCounts);
#ifndef USE_STRING_BLOCK
        int getPiece(std::vector<uint16_t> block, int index);
        std::vector<int> getPieceCounts(std::vector<uint16_t> puzzle);
        bool pieceCountIsValid(std::vector<uint16_t> puzzle);
        bool pieceCountIsValid(std::vector<uint16_t> puzzle, std::vector<int> count);
        bool checkAddition(std::vector<uint16_t> in, int inWidth, std::vector<uint16_t> added, int addedWidth);
        bool checkAddition(std::vector<uint16_t> in, int inWidth, std::vector<uint16_t> added, int addedWidth, std::vector<int> previousCounts);

        // Useful if we're having to load/save stringed blocks
        std::string utos (uint16_t block);
        uint16_t stou (std::string block);
#endif

        bool pieceHasTop(int piece)    { return ( (piece & 0x1) != 0 ); }
        bool pieceHasBottom(int piece) { return ( (piece & 0x2) != 0 ); }
        bool pieceHasLeft(int piece)   { return ( (piece & 0x4) != 0 ); }
        bool pieceHasRight(int piece)  { return ( (piece & 0x8) != 0 ); }


        // Puzzle data
        int height,
            width;
        std::vector<int> maxPieceCounts;
        unsigned int max_threads;
        std::mutex mutex_left;
        std::mutex mutex_valid;
#ifdef STORE_COMP
        std::shared_timed_mutex mutex_savingBlocks;
#endif

        // Vectors of blocks
#ifdef USE_STRING_BLOCK
        std::vector< std::string > leftBlocks;
        std::vector< std::string > midBlocks;
        std::vector< std::string > rightBlocks;

        std::vector< std::string > validSolutions;
#else
        std::vector< std::vector<uint16_t> > leftBlocks;
        std::vector< std::vector<uint16_t> > midBlocks;
        std::vector< std::vector<uint16_t> > rightBlocks;

        std::vector< std::vector<uint16_t> > validSolutions;
#endif

        // Storing data to files
        std::string leftBlockFileName  = "src/leftBlocks.txt" ;
        std::string midBlockFileName   = "src/midBlocks.txt"  ;
        std::string rightBlockFileName = "src/rightBlocks.txt";
        std::string validFileName      = "src/validBlocks.txt";

#ifdef USE_STRING_BLOCK
        void saveBlocks();
        void loadBlocks();
#endif
};

#endif
