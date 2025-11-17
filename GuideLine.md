# Term Project GuideLine

EECE667 2025 Fall Term Project in POSTECH

## !! Plagiarism Warning !!

Discussion among the student is allowed only in speech. However, sharing pseudo code or seeing others' code will be **strictly forbidden**. We will run the plagiarism detection program, and if it detects your code as positive and TA also determines that, then you will get an F grade for this course.

1. **Don't even see other student's code.**
2. **Don't share the pseudo code.** Only discussion in speech is allowed.

**Plagiarism** will be strictly reacted as **severe punishment**.

---

## Introduction

The parts that you should implement are two things; `Quadratic Placement` and `Your own placement`.

### 1. Quadratic Placement (30pt)

This is about what you learned in `Placement` Chapter of the class. You should implement Quadratic placement using a given data structure or using your own data structure.

You need to make a binary file whose name is `quadratic_placer` (This binary file has to be able to rebuild by TA), and that binary file should do a quadratic placement.

If you need more information about implementing a quadratic placer, refer to `doc/ref/QP.pdf`. This material is from the [Coursera Course](https://www.coursera.org/learn/vlsi-cad-layout), `VLSI CAD Part II: Layout`, from the `University of Illinois/NCSA`.

### 2. Your Own Placement (70pt)

In this part, you have to implement your own placement. The algorithm can be from your own idea, any journal, paper, or textbook. If you use other ideas from outside of your own, then annotate where the idea is from in the code.

### How To Set The Env?

In `Xshell`, `mobaXTerm`, or `Terminal`, enter the below command to access the POSTECH class server.

```sh
ssh -p 5900 <yourID>@class.postech.ac.kr -Y
```

For example,

```sh
ssh -p 5900 25EE66700@class.postech.ac.kr -Y
```

Your ID and PW will be given through `PLMS`. After you access the class server through your account, you should change your password with:

```sh
passwd
```

After accessing the server, copy the file and unzip.

```sh
cp ../25EE66700/TermProject.zip .
unzip TermProject.zip
cd TermProject
```

Load the required toolchain modules:

```sh
module load cmake/3.22.2
module load gcc/8.3.1
module load swig/3.0.2
```

Then follow the build section to generate your makefiles:

```sh
mkdir build
cd build
cmake ..
make
```

---

## Code Implementation

1. **You should submit a very brief report about your own code.** This will not affect your score; it will only be referenced to prevent plagiarism.
2. **Write your own comments in the code as much as you can.**

### 1. Quadratic Placement

Unless you use your own data structure, you only need to write your code in `void Circuit::quadraticPlacement()` in `src/algorithm/place.cpp`.

When you implement quadratic placement, you will need to solve the matrix:

$A x = b \tag{1}$

Here, the size of $A$ will be $n \times n$, where $n$ is the number of cells in the circuit. In average, $n \approx 2\times 10^5$; a dense matrix is infeasible. Use a [sparse matrix](https://en.wikipedia.org/wiki/Sparse_matrix) representation to solve the system.

The sparse matrix structure `coo_matrix` is provided. See `solve_example()` in `src/algorithm/math/matrixSolver.cpp` for usage. For example, the matrix

$$
A = \begin{pmatrix}
4.0 & -1.0 & 0\\
-1.0 & 4.0 & -1.0\\
0 & -1.0 & 4.0
\end{pmatrix} \tag{2}
$$

An example snippet:

```cpp
void solve_example() {
  // matrix solve example
  cout << endl << "** small demonstration **" << endl;
  coo_matrix A;
  int row_idx[] = {0, 0, 1, 1, 1, 2, 2};
  int col_idx[] = {0, 1, 0, 1, 2, 1, 2};
  double data[] = {4.0, -1.0, -1.0, 4.0, -1.0, -1.0, 4.0};

  int data_number = sizeof(row_idx) / sizeof(int);

  // initializing coo_matrix object
  A.n = 3; // 3x3 matrix
  A.nnz = data_number;
  A.row.resize(data_number);
  A.col.resize(data_number);
  A.dat.resize(data_number);

  // value inserting in coo_matrix object
  A.row = valarray<int>(row_idx, A.nnz);
  A.col = valarray<int>(col_idx, A.nnz);
  A.dat = valarray<double>(data, A.nnz);

  // initialize as [1, 1, 1] for golden solution
  valarray<double> x(1.0, A.n);
  valarray<double> b(A.n);
  A.matvec(x, b); // b = Ax

  cout << "b should equal [3,2,3]" << endl;
  cout << "b = "; print_valarray(b);

  // randomize x and solve
  for (int i = 0; i < A.n; ++i) x[i] = (double) random() / (double) RAND_MAX;

  cout << endl << "x = "; print_valarray(x);
  A.solve(b, x);
  cout << "after solve" << endl;
  cout << "x = "; print_valarray(x);
}
```

First, you should make the \(A\) matrix. When making \(A\), consider the hypergraph as a clique graph for easier implementation (star/clique modeling).

<img width="1185" height="509" alt="image" src="https://github.com/user-attachments/assets/73d107d0-cba7-410a-b911-4f38482ca7d6" />


#### How to Build

```sh
mkdir build
cd build
cmake ..
make
```

Run the quadratic placer:

```sh
./qPlacer <defname.def>
# e.g.
./qPlacer simple01.def
```

### 2. Your Own Placement

Implement your own placement. The algorithm can be from any journal, paper, textbook, or your own idea. If you use an external idea, annotate the source in the code.

You can use external libraries **only** for pure math (e.g., `fft`, `matrix solver`). If you want to use an external library, please notify the TA via PLMS.

#### How to Build

```sh
mkdir build
cd build
cmake ..
make
```

Run your placer:

```sh
./placer <defname.def>
# e.g.
./placer simple01.def
```

### Code Usages

See `example.cpp` for how to use `Circuit`, `Instance`, `Net`, and `Pin`. Also try calling `void Circuit::howToUse()` to print guidance in the console while reading code.

Frequently used APIs (see headers under `include/` for details):

- In `include/circuit/Circuit.h`
  - `void Circuit::quadraticPlacement()`
  - `void Circuit::myPlacement()`
  - `void Circuit::placeExample()`
  - `ulong Circuit::getHPWL()`

- In `include/dataStructures/Instance.h`
  - `string Instance::getName()`
  - `string Instance::getLibName()`
  - `uint Instance::getWidth()` / `getHeight()` / `getArea()`
  - `std::vector<Pin*> Instance::getPins()`
  - `pair<int,int> Instance::getCoordinate()`
  - `void Instance::setCoordinate(int x, int y)`
  - `bool Instance::isPlaced()`

- In `include/dataStructures/Net.h`
  - `string Net::getName()`
  - `vector<Pin*> Net::getConnectedPins()`
  - `int Net::getWeight()`
  - `string Net::getSignalType()`
  - `ulong Net::getHPWL()`

- In `include/dataStructures/Pin.h`
  - `bool isInstancePin()` / `bool isBlockPin()`
  - `Instance* getInstance()` / `Net* getNet()`
  - `string getSignalType()` / `string getPinName()`
  - `pair<int,int> getCoordinate()`

---

## Grade Criteria

The total score is 100 points: **Quadratic placement** and **Your own placement**.

All submissions will be graded only if the plagiarism check is passed. If a program (and TA review) finds plagiarism, you will receive F in this course.

### 1. Quadratic Placement (30pt)

**Due: 2025/12/08**

- Graded by HPWL value.
- If your HPWL equals the TA’s quadratic placer on each benchmark, you get full score for that case (6 points per benchmark).
- Allowed error: ±10% HPWL.
- If your HPWL is within the target scope, the TA will verify that your code really implements a quadratic placer; otherwise, zero score.

Build then evaluate:

```sh
mkdir build
cd build
cmake ..
make
./evaluator <defname.def> 0
# e.g.
./evaluator simple01.def 0
```

### 2. Your Own Placement (70pt)

**Due: 2025/11/29 and 2025/12/08**

The grading considers `HPWL` and `cell density` (routability).

### Cell Density (Routability) 
The evaluator makes a 40x40 grid and computes density as:
```
(total cell area in the grid) / (grid area)
```

If the **max** density over all grids exceeds 1.0, a penalty is applied; otherwise, density check is passed.

### Scoring

The score component evaluates the quality of your placement, based on HPWL and the density penalty.

The score follows:

$$
\text{PlacementScore} =
\begin{cases}
\alpha \times \dfrac{1}{1 + \log\left(\dfrac{\mathrm{HPWL}}{\mathrm{HPWL}_{ref}}\right)}, & \text{if maxDensity} \le 1 \\
\alpha \times \dfrac{1}{1 + \log\left(\dfrac{\mathrm{HPWL}}{\mathrm{HPWL}_{ref}}\right)} \times e^{-(\text{maxDensity}-1)}, & \text{otherwise}
\end{cases} \tag{3}
$$

#### 2.1 Part 1: Small Benchmarks

**Due: 2025/11/29**

Your task will be to create your own placer that will be evaluated on relatively small designs (`small01.def`, `small02.def`, and `small03.def`).

Runtime Limit: Your placer must be able to finish all placing all of the designs within 30 minutes. Designs that does not finish within this timeframe will receive 0 credit.


#### 2.2 Part 2: Large Benchmarks

**Due: 2025/12/08**

Then your next step will be to create a placer that will scale to larger designs (`large01.def`, `large02.def`, and `large03.def`).

Runtime Limit: Your placer must be able to finish all placing all of the designs within 60 minutes. Designs that does not finish within this timeframe will receive 0 credit.

#### How to Build and Evaluate

Build and evaluate (version 1 shows HPWL and max density):

```sh
mkdir build
cd build
cmake ..
make
./evaluator <defname.def> 1
# e.g.
./evaluator simple01.def 1
```

Or (using benchNumber)

```sh
mkdir build
cd build
cmake ..
make
# Place <benchNumber> benchmark using custom placer
./placer <benchNumber>
# e.g.
./placer 1
# Evaluate <benchNumber>
./evaluator <benchNumber> 1
# e.g.
./evaluator 1 1
```

### Benchmarks

| Part | Benchmark       | alpha | Remark | 
| --- | ---              | ---:  | ---:   |
| Part 1 | test.def      | 00    | Not evaluated  | 
| Part 1 | small01.def   | 05    | Open  | 
| Part 1 | small02.def   | 05    | Open  | 
| Part 1 | small03.def   | 05    | Hidden  | 
| Part 2 | large01.def   | 15    | Open  | 
| Part 2 | large02.def   | 20    | Open  | 
| Part 2 | large03.def   | 20    | Hidden  | 

Total sum of alphas is 70 points.

Scoring benchmarks: `small01.def`, `small02.def`, `small03.def`, `large01.def`, `large02.def`, and `large03.def`.

- `small01.def`, `small02.def`, `large01.def`, `large02.def` will be given.
- The other two (`small03.def` and `large03.def`) will be hidden.

Simple benchmarks for debugging (not for scoring): `test.def`, `simple01.def`, `simple02.def`, `simple03.def`, `simple04.def`.

**Total runtime limit**: 30 minutes to place all benchmarks.



#### Benchmark Designs

| Name | #inst | #net | #pad | Macro | Remark |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 25EECE667test | 122 | 311 | 0 | 0 | No eval |
| 25EECE667small1 | 3984 | 4122 | 267 | 0 | Open |
| 25EECE667small2 | 3716 | 3751 | 46 | 0 | Open |
| 25EECE667small3 | 7342 | 7598 | 369 | 0 | Hidden |
| 25EECE667large1 | 8283 | 8953 | 57 | 4 (Fixed) | Open |
| 25EECE667large2 | 72094 | 72410 | 1211 | 4 (Fixed) | Open |
| 25EECE667large3 | 35973 | 56700 | 1211 | 4 (Fixed) | Hidden |


---

## Submission

Submit your whole code **except** the CMake `build/` directory via the class server so that the TA can rebuild your binaries. Put the very brief report under the `doc/` directory.

## TA Contact

Please contact via PLMS.

## License

Copyright (c) 2025 CSDL (CAD&SoC Design Lab) POSTECH, Korea. All rights reserved.

```
Developed by:

  The teaching assistant of EECE667, POSTECH in Korea
```
