#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>

class Puzzle {
    public:
        // Constructors
        Puzzle();
        Puzzle(std::vector<int> pieceCountData, int puzzleHeight, int puzzleWidth);
        Puzzle(std::vector<int> pieceCountData, int puzzleHeight, int puzzleWidth, int desiredBlockWidth);

        // Destructors
        ~Puzzle();

        // Accessors
        int getPuzzleHeight();
        int getPuzzleWidth();
        int getDesiredLeftWidth();
        int getDesiredMidWidth();
        int getDesiredRightWidth();
        int getMaxThreads();

        void setMaxThreads(int threadCount) { if (threadCount > 0) max_threads = threadCount; }
        
        std::vector<std::string> getLeftBlocks();
        std::vector<std::string> getMidBlocks();
        std::vector<std::string> getRightBlocks();
        std::string getLeftBlocks(int index);
        std::string getMidBlocks(int index);
        std::string getRightBlocks(int index);

        int getLeftWidth();
        int getMidWidth();
        int getRightWidth();

        // Block creations
        void makeBlocks();

    private:
        // Functions related to block generation
        void generateFirstSet();
        void generateFirstSet(std::string in);

        void combineLeftBlocks();
        void combineRightBlocks();

        // Helper functions in generating
        int getPiece(std::string block, int index);
        std::vector<int> getPieceCounts(std::string puzzle);
        bool pieceCountIsValid(std::string puzzle);
        bool pieceCountIsValid(std::string puzzle, std::vector<int> count);

        bool pieceHasRight(int piece)  { return ( (piece & 0x8) != 0 ); }
        bool pieceHasLeft(int piece)   { return ( (piece & 0x4) != 0 ); }
        bool pieceHasTop(int piece)    { return ( (piece & 0x1) != 0 ); }
        bool pieceHasBottom(int piece) { return ( (piece & 0x2) != 0 ); }

        bool checkAddition(std::string in, int inWidth, std::string added, int addedWidth);
        bool checkAddition(std::string in, int inWidth, std::string added, int addedWidth, std::vector<int> previousCounts);


        // Puzzle data
        int height,
            width;
        std::vector<int> maxPieceCounts;
        unsigned int max_threads;
        std::mutex mutex1;

        // Vectors of blocks
        std::vector<std::string> leftBlocks;
        std::vector<std::string> midBlocks;
        std::vector<std::string> rightBlocks;
        std::vector<std::string> tempLeftBlocks;
        std::vector<std::string> tempMidBlocks;
        std::vector<std::string> tempRightBlocks;
        int leftWidth;
        int desiredLeftWidth;
        int midWidth;
        int desiredMidWidth;
        int rightWidth;
        int desiredRightWidth;

};
