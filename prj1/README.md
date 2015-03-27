-------------------------
Command Line Arguments
-------------------------

Example: ./predictors input.txt output.txt

------------------------
Documentation
------------------------

Used Doxygen syntax for documenting the code, making it easier to create a presentable document for easy understanding of code blocks.

-------------------------
Design Decisions & Issues
-------------------------
Highly considered using threads to speed up overall program, but due to time constraints and part of the program, like the tournament predictor, requires waiting on previous predictors, this was discarded.

Using the results generated from gShare and bimodal was an efficient choice so that the same code would not have to be repeated.

Used STL Vectors and STL bitsets for the State Tables and arrays, and was contemplating about the overhead involved; since it's not directly next to each other in heap.
While implementing the code, I was thinking of best ways of structuring it, which took longer than I intended.

Aside from improvements for micro-optimizing, perhaps the structural could be changed as the program takes quite a while to complete given a large amount of data.

There is an issue with the Tournament Predictor, largely logic errors. It ended up generating satisfiable results for me, so I am leaving it as it is for now.
