# Lyst Layout Solver, Python

Revisiting this project after 7 years. Had some ideas on how to speed things up, so doing a proof of concept in Python. When I set this project aside, I had the idea to find all possible vertical combinations of pieces, and then combine those together to reduce the number of checks needed. This idea was not completely implemented, so I'm doing that now.

Overview:
 - Construct each base piece
 - Create every vertical combination of pieces
   - Separate into left, center, and right pieces
 - Use backtracking to combine center pieces to find all combinations (in this case, 14 wide)
 - Then, find all valid combinations of left, (large) center, and right
