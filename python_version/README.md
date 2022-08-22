# Lyst Layout Solver, Python

Revisiting this project after 7 years. Had some ideas on how to speed things up, so doing a proof of concept in Python. When I set this project aside, I had the idea to find all possible vertical combinations of pieces, and then combine those together to reduce the number of checks needed. This idea was not completely implemented, so I'm doing that now.

Overview:
 - Construct each base piece
 - Create every vertical combination of pieces
   - Separate into left, center, and right pieces
 - Use backtracking to combine center pieces to find all combinations (in this case, 14 wide)
 - Then, find all valid combinations of left, (large) center, and right


## Benchmarks

Initial Python version shows it definitely works, but will take a while to run.
| Center Width | Combinations Found | Time (s) |
| ----------- | ----------- | ----------- |
| 2 | 1771 | 0.061 |
| 3 | 19448 | 0.257 |
| 4 | 176347 | 2.327 |
| 5 | 1317601 | 19.604 |
| 6 | 7896672 | 141.309 |
| 7 | 36927895 | 795.565 |

Using an exponential fit on this table, it is expected to take about 259.5 weeks to find all of the 14 wide combinations of center pieces.

## Using 2x3 blocks

Based on the max count of each piece, there are 154 different combinations of 1x3 center blocks. Using these 1x3 blocks, 1771 different 2x3 blocks can be created. Experimenting with the expected run times for the 2x3 blocks.
The maximum number of combinations for 1x3 blocks is 154^14^=4.2197*10^30^. When using 2x3 blocks, the maximum number of combinations is only 1771^7^=5.4642*10^22^. This increases the amount of RAM needed to hold the larger blocks, but should result in slightly faster runtime.
The following table shows a comparison of the two block methods. For lower center widths the values should be about the same, but at higher center widths more blocks should be trimmed as the maximum number of pieces takes effect.

| Center Width | 1x3 | 2x3 | Time (s) |
| ----------- | ----------- | ----------- | ----------- |
| 2 | 0.061 | 0.064 | 0.061 |
| 4 | 2.327 | 2.258 | 2.327 |
| 6 | 141.309 | 136.207 | 141.309 |
| 8 |  |  | 795.565 |
