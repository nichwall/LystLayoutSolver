#!/bin/python3

import itertools
from copy import deepcopy

# Piece class
class Piece:
    def __init__(self, value, identifier):
        # UP    : 0x01
        # DOWN  : 0x02
        # LEFT  : 0x04
        # RIGHT : 0x08
        self.up    = (value & 0x1) != 0
        self.down  = (value & 0x2) != 0
        self.left  = (value & 0x4) != 0
        self.right = (value & 0x8) != 0

        self.identifier = identifier # String identifier

    def __repr__(self):
        return self.identifier

class Block:
    def __init__(self, pieces):
        # Create block based on piece array.
        # Piece array is column major
        self.pieces = pieces

        self.left  = 0
        self.right = 0
        for i in range(3):
            if pieces[i].left:
                self.left  += 2**i
            right_idx = i + len(pieces) - 3
            if pieces[right_idx].right:
                self.right += 2**i

        self.piece_counts = [0]*16

    def __repr__(self):
        out = ""
        for i in self.pieces:
            out += str(i) + "\n"
        return out


# Create all piece elements
pieces = []
        # UP    : 0x01
        # DOWN  : 0x02
        # LEFT  : 0x04
        # RIGHT : 0x08
pieces.append(Piece(1, "^"))
pieces.append(Piece(2, "v"))
pieces.append(Piece(3, "|"))
pieces.append(Piece(4, "<"))
pieces.append(Piece(5, u'\u2518'))   #  0xD9 -> BOX DRAWINGS LIGHT UP AND LEFT
pieces.append(Piece(6, u'\u2510'))   #  0xBF -> BOX DRAWINGS LIGHT DOWN AND LEFT
pieces.append(Piece(7, u'\u2524'))   #  0xB4 -> BOX DRAWINGS LIGHT VERTICAL AND LEFT
pieces.append(Piece(8, ">"))
pieces.append(Piece(9, u'\u2514'))   #  0xC0 -> BOX DRAWINGS LIGHT UP AND RIGHT
pieces.append(Piece(10,u'\u250c'))   #  0xDA -> BOX DRAWINGS LIGHT DOWN AND RIGHT
pieces.append(Piece(11,u'\u251c'))   #  0xC3 -> BOX DRAWINGS LIGHT VERTICAL AND RIGHT
pieces.append(Piece(12,u'\u2500'))   #  0xC4 -> BOX DRAWINGS LIGHT HORIZONTAL
pieces.append(Piece(13,u'\u2534'))   #  0xC1 -> BOX DRAWINGS LIGHT UP AND HORIZONTAL
pieces.append(Piece(14,u'\u252c'))   #  0xC2 -> BOX DRAWINGS LIGHT DOWN AND HORIZONTAL
pieces.append(Piece(15,u'\u253c'))   #  0xC5 -> BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL

for i in range(len(pieces)):
    print(pieces[i])

HEIGHT = 3
WIDTH  = 16

# Create each "block"
blocks_of_combinations = [p for p in itertools.product(pieces, repeat=3)]
print(f"Total number: {len(blocks_of_combinations)}")
# Check if valid combination, and which set to put it in
left   = []
center = []
right  = []

def block_is_valid(block):
    pieces = block.pieces
    # Iterate over each piece to see if valid. Assumes a single column
    # First element is top, last element is bottom.
    if pieces[0].up or pieces[-1].down:
        return False
    # Check middle pieces
    for idx in range(1,len(pieces)):
        if pieces[idx].up != pieces[idx-1].down:
            return False
    # Is valid
    return True

# Horizontally concat multiline strings
def horz_cat_string(str1, str2):
    splt_lines = zip(str1.split('\n'), str2.split('\n'))
    # Horizontal join
    res = '\n'.join([x+y for x,y in splt_lines])
    return res

for piece_arr in blocks_of_combinations:
    # Convert piece array to block
    block = Block(piece_arr)

    if not block_is_valid(block):
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

print(f"  Left blocks: {len(left)}")
print(f" Right blocks: {len(right)}")
print(f"Center blocks: {len(center)}")

print("\n\nLeft blocks")
left_str = "\n\n"
for i in left:
    left_str = horz_cat_string(left_str, str(i))
    left_str = horz_cat_string(left_str, "  \n  \n  ")
print(left_str)

print("\n\nRight blocks")
right_str = "\n\n"
for i in right:
    right_str = horz_cat_string(right_str, str(i))
    right_str = horz_cat_string(right_str, "  \n  \n  ")
print(right_str)
