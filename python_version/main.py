#!/bin/python3

from copy import deepcopy

from Globals import *
from Piece import *
from Block import *

############################################
# Create pieces
############################################
pieces = create_pieces()

for i in range(len(pieces)):
    print(f"{pieces[i].value}: {pieces[i]}    max count: {MAX_COUNTS[i]}")


############################################
# Create blocks
############################################
[left, center, right] = create_vertical_blocks(pieces)

############################################
# Printing block stats
############################################
print(f"  Left blocks: {len(left)}")
print(f" Right blocks: {len(right)}")
print(f"Center blocks: {len(center)}")

print("\nLeft blocks")
print_blocks(left)

print("\nRight blocks")
print_blocks(right)

print("\nCenter blocks")
print_blocks(center)

############################################
# Combine center blocks
############################################
