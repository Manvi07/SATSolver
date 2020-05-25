# SATSolver
A basic implementation of the DPLL algorithm with unit propagation and backtracking to check if a given set of clauses is Satisfiable and also provide a satisfying assigment if that's the case. 

#### Input file format: [DIMACS](http://www.cs.utexas.edu/users/moore/acl2/manuals/current/manual/index-seo.php/SATLINK____DIMACS)

#### To run the SAT Solver:
##### (Reqd. C++17)
Step 1 ```./build.sh``` </br >
Step 2 ```./solve.sh <input-filename>``` 

#### Output format:
<b>SAT</b>: outputs `sat` and returns with exit code 10.
<b>UNSAT</b>: outputs `unsat` and returns with exit code 20.
