
from Globals import *

import itertools

class Block:
    def __init__(self, pieces):
        # Create block based on piece array.
        # Piece array is column major
        self.pieces = pieces
        self.width = len(pieces)/HEIGHT

        self.left  = 0
        self.right = 0
        for i in range(3):
            if pieces[i].left:
                self.left  += 2**i
            right_idx = i + len(pieces) - 3
            if pieces[right_idx].right:
                self.right += 2**i

        self.piece_counts = [0]*16
        # Update piece counts by piece value
        for p in pieces:
            self.piece_counts[p.value] += 1

    def __repr__(self):
        out = ""
        for j in range(HEIGHT):
            for i in range(j, len(self.pieces), HEIGHT):
                out += str(self.pieces[i])
            out += "\n"
        return out

    def add_block(self, right_block):
        # DONT USE. Found to be slower than
        # just creating a new block
        for p in right_block.pieces:
            self.piece_counts[p.value] += 1

        self.pieces += right_block.pieces
        self.right = right_block.right
        self.width += right_block.width

def vertical_block_is_valid(block):
    pieces = block.pieces
    # Iterate over each piece to see if valid. Assumes a single column
    # First element is top, last element is bottom.
    if pieces[0].up or pieces[-1].down:
        return False
    # Check middle pieces
    for idx in range(1,len(pieces)):
        if pieces[idx].up != pieces[idx-1].down:
            return False

    # Check if counts are less than maximum allowed
    for i in range(len(pieces)):
        if block.piece_counts[i] > MAX_COUNTS[i]:
            return False
    # Is valid
    return True

# Create each possible vertical block combination,
# then separate into left, center, and right arrays
def create_vertical_blocks(pieces):
    # Create each "block"
    blocks_of_combinations = [p for p in itertools.product(pieces, repeat=HEIGHT)]
    print(f"Total number: {len(blocks_of_combinations)}")
    # Check if valid combination, and which set to put it in
    left   = []
    center = []
    right  = []

    for piece_arr in blocks_of_combinations:
        # Convert piece array to block
        block = Block(piece_arr)

        if not vertical_block_is_valid(block):
            continue

        # Check if left block
        if block.left == 0 and block.right != 0:
            left.append(block)
        # Check if right block
        if block.left != 0 and block.right == 0:
            right.append(block)
        # Check if center block
        if block.left != 0 and block.right != 0:
            center.append(block)
    return [left, center, right]

def print_blocks(block_array, count_per_row=30):
    for j in range( int(len(block_array)/count_per_row)+1 ):
        block_str = "\n\n"
        for i in block_array[j*count_per_row:(j+1)*count_per_row]:
            block_str = horz_cat_string(block_str, str(i))
            block_str = horz_cat_string(block_str, "  \n  \n  ")
        print(block_str)
        print()

def print_stats(left, center, right):
    print(f"  Left blocks: {len(left)}")
    print(f" Right blocks: {len(right)}")
    print(f"Center blocks: {len(center)}")
    print("\nLeft blocks")
    print_blocks(left)
    print("\nRight blocks")
    print_blocks(right)
    print("\nCenter blocks")
    print_blocks(center)

# Combine two blocks together
def can_combine_blocks(left_block, right_block):
    # Check if sides match up
    if left_block.right != right_block.left:
        return False

    # Check if counts are too big
    for idx in range(len(MAX_COUNTS)):
        if left_block.piece_counts[idx] + right_block.piece_counts[idx] > MAX_COUNTS[idx]:
            return False

    return True
