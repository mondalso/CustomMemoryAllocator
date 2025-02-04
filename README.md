# Custom Memory Allocator

## Overview
This repository contains implementations of custom memory allocation strategies in C, including a **naive malloc**, **first-fit malloc**, and **best-fit malloc**.

## Implementations
### 1. Naive Malloc (`naive_malloc.c`)
- Uses `sbrk()` to request memory from the OS.
- Does not implement a free list or reuse memory efficiently.
- Each allocation request increases the program's break, making it inefficient.
- **Significance:** Simple implementation but inefficient due to lack of memory reuse and fragmentation handling.

### 2. First-Fit Malloc (`firstfit_malloc.c`)
- Implements a **basic heap with a fixed size**.
- Searches for the first available memory block that fits the requested size.
- Allocates and marks the block as used.
- Uses an in-memory heap simulation instead of `sbrk()`.
- **Significance:** Faster allocation due to the first available fit approach but may cause fragmentation.

### 3. First-Fit & Best-Fit Malloc (`firstfit_bestfit_malloc.c`)
- Implements both **first-fit** and **best-fit** allocation strategies.
- Splits large blocks to minimize fragmentation.
- Merges adjacent free blocks to improve memory reuse.
- **Significance:** Best-fit minimizes fragmentation by choosing the smallest suitable block, but may lead to longer search times.

## Features
- Simulates memory allocation without using `malloc()`.
- Implements simple **memory management techniques**.
- Supports **freeing memory** and **coalescing free blocks**.

## Compilation and Execution
To compile any of the implementations, use:
```sh
gcc naive_malloc.c -o naive_malloc
./naive_malloc
```
For first-fit allocation:
```sh
gcc firstfit_malloc.c -o firstfit_malloc
./firstfit_malloc
```
For first-fit & best-fit allocation:
```sh
gcc firstfit_bestfit_malloc.c -o firstfit_bestfit_malloc
./firstfit_bestfit_malloc
```

## Expected Output
The programs print the status of memory allocation and deallocation, showing how blocks are allocated and freed over time.

## Future Improvements
- Implement a **buddy system allocator** for better fragmentation handling.
- Add **thread safety** using locks for concurrent memory allocation.
- Use **metadata compression** to optimize heap usage.

## License
This project is open-source and available under the **MIT License**.

