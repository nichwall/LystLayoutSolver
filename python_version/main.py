#!/bin/python3

from copy import deepcopy

from Globals import *
from Piece import *
from Block import *

############################################
# Create pieces
############################################
pieces = create_pieces()

#for i in range(len(pieces)):
    #print(f"{pieces[i].value}: {pieces[i]}    max count: {MAX_COUNTS[i]}")

############################################
# Create vertical blocks
############################################
[left, center, right] = create_vertical_blocks(pieces)

############################################
# Printing vertical block stats
############################################
#print_stats(left, center, right)

############################################
# Combine center blocks
############################################
center_blocks = center
valid_found = 0
def combine_center(current_block):
    global valid_found
    # Recursive iterate to find all valid blocks
    if current_block.width == WIDTH-2:
        #print("FOUND A MATCH")
        #print(current_block)
        valid_found += 1
        return

    # Iterate over all of the center blocks
    for next_block in center_blocks:
        # Check if next_block can be combined with current_block
        if can_combine_blocks(current_block, next_block):
            combine_center( Block( current_block.pieces + next_block.pieces ) )
            #passed_block = deepcopy(current_block)
            #passed_block.add_block(next_block) 
            #combine_center( passed_block )

# Create each possible block combination of size Nx3
larger_blocks = []
def create_larger_blocks(current_block, width=1):
    global valid_found
    global larger_blocks
    # Recursive iterate to find all valid blocks
    if current_block.width == width:
        valid_found += 1
        larger_blocks.append(current_block)
        return

    # Iterate over all of the center blocks
    for next_block in center_blocks:
        # Check if next_block can be combined with current_block
        if can_combine_blocks(current_block, next_block):
            create_larger_blocks( Block( current_block.pieces + next_block.pieces ), width )

# Make larger blocks
#for first_block in center_blocks:
    #create_larger_blocks(first_block, width=2)

print(f" Length of larger_blocks: {len(larger_blocks)}")

valid_found = 0
for first_block in center_blocks:
#for first_block in larger_blocks:
    combine_center(first_block)

print(f"Found {valid_found} solutions of width {WIDTH-2} center blocks")

