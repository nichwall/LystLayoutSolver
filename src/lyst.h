#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <algorithm>
#include <math.h>

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
        
        std::vector<std::string> getLeftBlocks();
        std::vector<std::string> getMidBlocks();
        std::vector<std::string> getRightBlocks();
        std::string getLeftBlocks(int index);
        std::string getMidBlocks(int index);
        std::string getRightBlocks(int index);
        /*std::vector< std::vector<uint16_t> > getLeftBlocks();
        std::vector< std::vector<uint16_t> > getRightBlocks();
        std::vector< std::vector<uint16_t> > getMidBlocks();
        std::vector<uint16_t> getLeftBlocks(int index);
        std::vector<uint16_t> getRightBlocks(int index);
        std::vector<uint16_t> getMidBlocks(int index);*/

        // Block creations
        void makeBlocks();

    private:
        // Functions related to block generation
        void generateFirstSet();
        void generateFirstSet(std::string in);

        void combineLeftBlocks();

        // Helper functions in generating
        int getPiece(std::vector<uint16_t> block, int index);
        int getPiece(std::string block, int index);
        std::vector<int> getPieceCounts(std::vector<uint16_t> puzzle);
        bool pieceCountIsValid(std::string puzzle);
        bool pieceCountIsValid(std::string puzzle, std::vector<int> count);
        bool pieceCountIsValid(std::vector<uint16_t> puzzle);
        bool pieceCountIsValid(std::vector<uint16_t> puzzle, std::vector<int> count);

        bool pieceHasRight(int piece)  { return ( (piece & 0x8) != 0 ); }
        bool pieceHasLeft(int piece)   { return ( (piece & 0x4) != 0 ); }
        bool pieceHasTop(int piece)    { return ( (piece & 0x1) != 0 ); }
        bool pieceHasBottom(int piece) { return ( (piece & 0x2) != 0 ); }

        bool checkAddition(std::vector<uint16_t> in, int inWidth, std::vector<uint16_t> added, int addedWidth);
        bool checkAddition(std::vector<uint16_t> in, int inWidth, std::vector<uint16_t> added, int addedWidth, std::vector<int> previousCounts);
        bool checkAddition(std::string in, int inWidth, std::string added, int addedWidth);
        bool checkAddition(std::string in, int inWidth, std::string added, int addedWidth, std::vector<int> previousCounts);


        // Puzzle data
        int height,
            width;
        std::vector<int> maxPieceCounts;
        unsigned int max_threads;
        std::mutex mutex_left;
        std::mutex mutex_valid;
        std::shared_timed_mutex mutex_savingBlocks;

        // Vectors of blocks
        /*std::vector< std::vector<uint16_t> > leftBlocks;
        std::vector< std::vector<uint16_t> > midBlocks;
        std::vector< std::vector<uint16_t> > rightBlocks;*/
        std::vector< std::string > leftBlocks;
        std::vector< std::string > midBlocks;
        std::vector< std::string > rightBlocks;

        std::vector< std::string > validSolutions;

        // Storing data to files
        std::string leftBlockFileName  = "src/leftBlocks.txt" ;
        std::string midBlockFileName   = "src/midBlocks.txt"  ;
        std::string rightBlockFileName = "src/rightBlocks.txt";
        std::string validFileName      = "src/validBlocks.txt";

        void saveBlocks();
        void loadBlocks();
};
