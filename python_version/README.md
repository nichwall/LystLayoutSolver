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
