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

class Puzzle {
    public:
        // Constructors
        Puzzle();
        Puzzle(std::vector<int> pieceCountData, int puzzleHeight, int puzzleWidth);

        // Destructors
        ~Puzzle();

        // Accessors
        inline int getPuzzleHeight() { return height;      }
        inline int getPuzzleWidth()  { return width;       }
        inline int getMaxThreads()   { return max_threads; }
        inline bool isLoading()      { return loading;     }

        inline void setMaxThreads(int threadCount) { if (threadCount > 0) max_threads = threadCount; }
        inline void setVerbosity(int verbosity) { if (verbosity > 0) verbosity_level = verbosity; }
        inline void setLoading(bool leading)    { this->loading = loading; }
        
        inline std::vector<int> getMaxPieceCount() { return maxPieceCounts; }
#ifdef USE_STRING_BLOCK
        inline std::vector<std::string> getLeftBlocks()  { return leftBlocks;     }
        inline std::vector<std::string> getMidBlocks()   { return midBlocks;      }
        inline std::vector<std::string> getRightBlocks() { return rightBlocks;    }
        inline std::vector<std::string> getValidBlocks() { return validSolutions; }
        inline std::string getLeftBlocks(int index)      { return leftBlocks[index];     }
        inline std::string getMidBlocks(int index)       { return midBlocks[index];      }
        inline std::string getRightBlocks(int index)     { return rightBlocks[index];    }
        inline std::string getValidBlocks(int index)     { return validSolutions[index]; }
#else
        inline std::vector< std::vector<uint16_t> > getLeftBlocks()  { return leftBlocks;     }
        inline std::vector< std::vector<uint16_t> > getMidBlocks()   { return midBlocks;      }
        inline std::vector< std::vector<uint16_t> > getRightBlocks() { return rightBlocks;    }
        inline std::vector< std::vector<uint16_t> > getValidBlocks() { return validSolutions; }
        inline std::vector<uint16_t> getLeftBlocks(int index)        { return leftBlocks[index];     }
        inline std::vector<uint16_t> getMidBlocks(int index)         { return midBlocks[index];      }
        inline std::vector<uint16_t> getRightBlocks(int index)       { return rightBlocks[index];    }
        inline std::vector<uint16_t> getValidBlocks(int index)       { return validSolutions[index]; }
#endif

        // Block creations
        void makeBlocks(bool = false);

    private:
        // Functions related to block generation
        void generateFirstSet();
        void generateFirstSet(std::string in);

        void combineLeftBlocks();

        // Helper functions in generating
        int getPiece(std::string block, unsigned int index);
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

        inline bool pieceHasTop(int piece)    { return ( (piece & 0x1) != 0 ); }
        inline bool pieceHasBottom(int piece) { return ( (piece & 0x2) != 0 ); }
        inline bool pieceHasLeft(int piece)   { return ( (piece & 0x4) != 0 ); }
        inline bool pieceHasRight(int piece)  { return ( (piece & 0x8) != 0 ); }

        // Puzzle data
        unsigned int height,
                     width;
        std::vector<int> maxPieceCounts;
        unsigned int max_threads;
        unsigned int verbosity_level;
        std::mutex mutex_left;
        std::mutex mutex_valid;
        bool loading;

        // Vectors of blocks
#ifdef USE_STRING_BLOCK
        std::vector< std::string > leftBlocks;
        std::vector< std::string > midBlocks;
        std::vector< std::string > rightBlocks;

        std::vector< std::string > validSolutions;

        void saveLeft(); // Saves left blocks in order to continue for next run
        std::ofstream solutions;
        void writeSolutions();
#ifdef BREADTH_SEARCH
        std::vector< std::string > tempLeft;
        int leftWidth;
#endif
#else
        std::vector< std::vector<uint16_t> > leftBlocks;
        std::vector< std::vector<uint16_t> > midBlocks;
        std::vector< std::vector<uint16_t> > rightBlocks;

        std::vector< std::vector<uint16_t> > validSolutions;
#ifdef BREADTH_SEARCH
        std::vector< std::vector<uint16_t> > tempLeft;
        int leftWidth;
#endif
#endif
};

#endif
