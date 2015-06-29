#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <algorithm>    // std::sort
#include <bitset>
#include <inttypes.h>	// intXX_t
#include <fstream>		// File writes

// Old Blocks
std::vector<std::string> leftBlock;
int leftWidth = 1;
std::vector<std::string> rightBlock;
int rightWidth = 1;
std::vector<std::string> midBlock;
int midWidth = 1;

// Backup config
std::string leftFileName = "leftBlocks.txt";
std::string midFileName = "midBlocks.txt";
std::string rightFileName = "rightBlocks.txt";
std::string iterationFileName = "lastIterationCount.txt";
int rootIt = 0;
// End config

// Solutions!
std::vector<std::string> solutions;
std::ofstream outFile ("solutions.txt");

// Puzzle info
int puzzleHeight = 3;
int puzzleWidth = 16;
//int puzzleHeight = 16;
//int puzzleWidth = 3;
int numCounts [] = {1000,4,5,8,4,2,2,5,1,5,3,5,2,1,1,0};
// Block info
int blockWidth = 2;
int blockHeight = 3;
int blockLen = blockHeight*blockWidth;

int blockCount = (puzzleHeight/blockHeight) * (puzzleWidth/blockWidth);
int maxLevel;

// Saving state
void saveBlocks() {
	std::ofstream leftFile (leftFileName.c_str());
	if (leftFile.is_open()) {
		for (int i=0; i<leftBlock.size(); i++) {
			leftFile << leftBlock.at(i) << "\n";
		}
		leftFile.close();
	}
	std::ofstream midFile (midFileName.c_str());
	if (midFile.is_open()) {
		for (int i=0; i<midBlock.size(); i++) {
			midFile << midBlock.at(i) << "\n";
		}
		midFile.close();
	}
	std::ofstream rightFile (rightFileName.c_str());
	if (rightFile.is_open()) {
		for (int i=0; i<rightBlock.size(); i++) {
			rightFile << rightBlock.at(i) << "\n";
		}
		rightFile.close();
	}
}
void saveRootIt(int iteration) {
	std::ofstream itFile (iterationFileName.c_str());
	if (itFile.is_open()) {
		itFile << iteration;
		itFile.close();
	}
}
// Loading state
void loadBlocks() {
	std::ifstream leftFile (leftFileName.c_str());
	std::string line;
	if (leftFile.is_open()) {
		while ( getline(leftFile,line) ) {
			leftBlock.push_back(line);
		}
		leftFile.close();
	}
	std::ifstream midFile (midFileName.c_str());
	if (midFile.is_open()) {
		while ( getline(midFile,line) ) {
			midBlock.push_back(line);
		}
		midFile.close();
	}
	std::ifstream rightFile (rightFileName.c_str());
	if (rightFile.is_open()) {
		while ( getline(rightFile,line) ) {
			rightBlock.push_back(line);
		}
		rightFile.close();
	}
}
void loadRootIt() {
	std::ifstream itFile (iterationFileName.c_str());
	std::string line;
	if (itFile.is_open()) {
		getline(itFile,line);
		rootIt = atoi(line.c_str());
		itFile.close();
	}
}

// Get the piece at the desired index
int getPiece(std::string block, int index) {
	if (index >= block.length()) {
		return 0;
	}
	
	int piece = (int)block[index]-96;
	return piece;
}
int* getPieceCounts(std::string puzzle) {
	int *count = new int[16];
	for (int i=0; i<puzzle.length(); i++) {
		int temp = getPiece(puzzle,i);
		count[temp]++;
	}
	return count;
}
int pieceCountIsValid(std::string puzzle) {
	int count [16] = {0};
	for (int i=0; i<puzzle.length(); i++) {
		int temp = getPiece(puzzle,i);
		count[temp]++;
		if (count[temp] > numCounts[temp]) {
			return 3;
		}
	}
	return 0;
}

/* 0x1:	Up
 * 0x2:	Down
 * 0x4:	Left
 * 0x8: Right */
int pieceHasRight(int piece) {
	return ( (piece & 0x8)!=0 );
}
int pieceHasLeft(int piece) {
	return ( (piece & 0x4)!=0 );
}
int pieceHasTop(int piece) {
	return ( (piece & 0x1)!=0 );
}
int pieceHasBottom(int piece) {
	return ( (piece & 0x2)!=0 );
}

// Check whether the new addition is valid
int checkAddition(std::string in, int inWidth, std::string added, int addedWidth) {
	for (int i=0; i<blockHeight; i++) {
		int leftPiece = getPiece(in, (i+1)*inWidth-1);
		int rightPiece = getPiece(added, i*addedWidth);
		if (pieceHasRight(leftPiece) != pieceHasLeft(rightPiece)) {
			return 1;
		}
		// Make sure that we don't have H/8 to the left of D/4
		if (leftPiece==8 && rightPiece==4) {
			return 2;
		}
	}
	
	// Check piece counts
	return pieceCountIsValid(in+added);
}

// Recursively create every possible column
// Will check to make sure that the column does not have a self
// contained island
void generateVertical(std::string in) {
	// Time to add!
	if (in.length() >= puzzleHeight) {
		// Check bottom
		if (pieceHasBottom( getPiece(in,puzzleHeight-1) )) {
			return;
		}
		
		int r=0, l=0;
		for (int i=0; i<in.length(); i++) {
			int piece = getPiece(in,i);
			// Figure out whether left, right, or middle piece
			if (pieceHasRight(piece)) {
				r = 1;
			}
			if (pieceHasLeft(piece)) {
				l = 1;
			}
		}
		// If it's neither left or right, then it's a bad piece
		if (r==0 && l==0) {
			return;
		}
		// Check if right block
		if (r==0) {
			rightBlock.push_back(in);
			return;
		}
		// Check if left block
		if (l==0) {
			leftBlock.push_back(in);
			return;
		}
		midBlock.push_back(in);
		return;
	}
	
	for (int i=1; i<15; i++) {
		// Check if the piece matches the above state
		int abovePiece = 0;
		if (in.length() != 0) {
			abovePiece = getPiece(in,in.length()-1);
		}
		if ( pieceHasBottom(abovePiece) != pieceHasTop(i) ) {
			continue;
		}
		
		// Check that A is not below B
		if (i==1 && abovePiece==2) {
			continue;
		}
		
		// Check if we have too many of a piece
		int counts[16] = {0};
		int broken = 0;
		for (int j=0; j<in.length(); j++) {
			int temp = getPiece(in,j);
			counts[temp]++;
			if (counts[temp] > numCounts[temp]) {
				broken = 1;
				break;
			}
		}
		if (broken == 1) {
			continue;
		}
				
		// No errors occured, so recurse!
		generateVertical( in + (char)(i+96) );
	}
	return;
}

// Combines left with mid, and then replaces left
void combineLeft() {
	std::vector<std::string> newLeft;
	// Check that we need to combine the middle, not the right
	if (leftWidth+rightWidth >= puzzleWidth) {
		for (int i=0; i<leftBlock.size(); i++) {
			std::cout << "LeftBlock: " << i << "/"<<leftBlock.size()<<"\n";
			for (int j=0; j<rightBlock.size(); j++) {
				// Check whether it's good
				if (checkAddition(leftBlock.at(i), leftWidth, rightBlock.at(j), rightWidth) != 0) {
					continue;
				}
				// Otherwise, add the combined blocks to "newLeft"
				newLeft.push_back(leftBlock.at(i)+rightBlock.at(j));
			}
		}
	} else {
		for (int i=0; i<leftBlock.size(); i++) {
			for (int j=0; j<midBlock.size(); j++) {
				// Check whether it's good
				if (checkAddition(leftBlock.at(i), leftWidth, midBlock.at(j), midWidth) != 0) {
					continue;
				}
				// Otherwise, add the combined blocks to "newLeft"
				newLeft.push_back(leftBlock.at(i)+midBlock.at(j));
			}
		}
	}
	
	
	leftBlock.erase(leftBlock.begin(), leftBlock.end());
	for (int i=0; i<newLeft.size(); i++) {
		leftBlock.push_back(newLeft.at(i));
	}
	printf("\nNew Left count: %d\n",newLeft.size());
	
	leftWidth += midWidth;
	return;
}
void combineMid() {
	std::vector<std::string> newMid;
	for (int i=0; i<midBlock.size(); i++) {
		for (int j=0; j<midBlock.size(); j++) {
			// Check whether it's good
			if (checkAddition(midBlock.at(i), midWidth, midBlock.at(j), midWidth) != 0) {
				continue;
			}
			// Otherwise, add the combined blocks to "newLeft"
			newMid.push_back(midBlock.at(i)+midBlock.at(j));
		}
	}
	
	
	midBlock.erase(midBlock.begin(), midBlock.end());
	for (int i=0; i<newMid.size(); i++) {
		midBlock.push_back(newMid.at(i));
	}
	printf("New Mid count: %d\n",newMid.size());
	
	midWidth += midWidth;
	return;
}
void combineRight() {
	std::vector<std::string> newRight;
	for (int i=0; i<midBlock.size(); i++) {
		for (int j=0; j<rightBlock.size(); j++) {
			// Check whether it's good
			if (checkAddition(midBlock.at(i), midWidth, rightBlock.at(j), rightWidth) != 0) {
				continue;
			}
			// Otherwise, add the combined blocks to "newLeft"
			newRight.push_back(midBlock.at(i)+rightBlock.at(j));
		}
	}
	
	
	rightBlock.erase(rightBlock.begin(), rightBlock.end());
	for (int i=0; i<newRight.size(); i++) {
		rightBlock.push_back(newRight.at(i));
	}
	printf("New Right count: %d\n",newRight.size());
	
	rightWidth += midWidth;
	return;
}

// Solve the puzzle!
void solver(std::string in, int level) {
	if (level >= maxLevel) {
		std::cout << "Found a solution!  " << in << "\n";
		solutions.push_back(in);
		// Write solutions to a file
		if (outFile.is_open()) {
			outFile << in << "\n";
		}
		return;
	}
	
	// Check if we're at a middle level, because those are the most common
	if (level>0 && level+1<maxLevel) {
		// Middle levels
		for (int i=0; i<midBlock.size(); i++) {
			if (checkAddition(in, leftWidth+(level*midWidth), midBlock.at(i), midWidth) == 0) {
				//std::cout << "Level... "<<level<<"  "<<i<<"/"<<midBlock.size()<<"\n";
				if (level==1) {
					std::cout << "Level... "<<level<<"  "<<i<<"/"<<midBlock.size()<<"\n";
				}
				solver(in+midBlock.at(i) , level+1);
			}
		}
	} else if (level==0) {
		// Root level
		for (int i=0; i<leftBlock.size(); i++) {
			if ( i <= rootIt ) {
				continue;
			}
			std::cout << "Root level... "<<i<<"/"<<leftBlock.size()<<"\tSolutions: "<<solutions.size()<<"\n";
			solver(leftBlock.at(i), 1);
			saveRootIt(i);
		}
	} else if (level<maxLevel){
		// Final level
		for (int i=0; i<rightBlock.size(); i++) {
			if (checkAddition(in, puzzleWidth-rightWidth, rightBlock.at(i), rightWidth) == 0) {
				solver(in+rightBlock.at(i),level+1);
			}
		}
	}
	return;
}

int main() {
	int restoreState = 1;
	
	if (restoreState == 0) {
		generateVertical("");
		
		printf("LeftBlock count: %d\n",leftBlock.size());
		printf("RightBlock count: %d\n",rightBlock.size());
		printf("MidBlock count: %d\n",midBlock.size());
		
		for (int i=0; i<2; i++) {
			printf("Running iteration %d...\n",i);
			combineLeft();
			combineRight();
			combineMid();
		}
		//combineLeft();
		//combineRight();
		
		printf("sorting...\n");
		std::sort(leftBlock.begin(), leftBlock.end());
		std::sort(rightBlock.begin(), rightBlock.end());
		std::sort(midBlock.begin(), midBlock.end());
		
		// Output blocks to file to speed up startup
		saveBlocks();
	} else {
		// Load blocks from last run
		loadBlocks();
		// Load last iteration
		loadRootIt();
	}
	
	printf("\nLeftBlock count: %d\n",leftBlock.size());
	printf("RightBlock count: %d\n",rightBlock.size());
	printf("MidBlock count: %d\n",midBlock.size());
	std::cout << "L: "<<leftWidth<<"  M: "<<midWidth<<"  R: "<<rightWidth<<"\n";
	
	maxLevel = (puzzleWidth-leftWidth-rightWidth)/midWidth + 2;
	std::cout << "Max Level: " << maxLevel << "\n";
	solver("",0);
	
	// Close solutions file
	outFile.close();
	return 0;
}
