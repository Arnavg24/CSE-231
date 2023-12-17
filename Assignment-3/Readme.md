# MeMS - Memory Management System

## Problem Statement

Implement a custom memory management system (MeMS) in C using system calls `mmap` and `munmap`. MeMS must adhere to specific constraints and requirements:

### Constraints and Requirements:

- Use only `mmap` and `munmap` for memory management.
- Allocate memory in multiples of the system's PAGE_SIZE (e.g., 4 KB).
- Deallocate memory only in multiples of PAGE_SIZE.
- Utilize a free list data structure.
- MeMS manages heap memory for only one process at a time.
- Allocate/deallocate memory through MeMS functions (not using other memory management library functions).

### Free List Structure:

- Represented as a doubly linked list with a main chain and sub-chains.
- Tracks allocated memory (PROCESS) and unallocated memory (HOLE).

### MeMS Virtual Address and MeMS Physical Address:

- MeMS manages a mapping from MeMS virtual address space to MeMS physical address space.

### Function Implementations:

- `mems_init()`: Initializes MeMS system parameters.
- `mems_finish()`: Unmaps allocated memory.
- `mems_malloc(size_t size)`: Allocates memory.
- `mems_free(void* ptr)`: Frees memory.
- `mems_print_stats()`: Prints memory stats and details.
- `mems_get(void* v_ptr)`: Returns MeMS physical address corresponding to MeMS virtual address.

## Submission Guidelines

- Download the skeleton template from the provided GitHub repository.
- Implement functions in the provided C code.
- Include a detailed explanation of the approach.
- Demonstrate functionality with test cases.
- Do not change function names or signatures.
- Documentation, error handling, and covering edge cases are crucial for grading.
- Use the `PAGE_SIZE` macro and GitHub for version control.

## Grading Criteria

- Documentation quality.
- Viva/Demo performance.
- Error handling and edge case coverage.
- Correct mapping between MeMS virtual and physical addresses.
- Successful compilation and correct output format.
- Use the prescribed data structures and guidelines.

### Note and Suggestions

- Utilize the `PAGE_SIZE` macro.
- Maintain proper program structure and naming conventions.
- Utilize GitHub for version control.

## Additional Guidelines (Added on October 28th, 2023)

- Grading based on documentation, viva/demo, error handling, edge case coverage, address mapping, code compilation, and adherence to guidelines.
- Utilize data structures as described in the documentation.

*Note: Maintain good program structure and adhere to prescribed guidelines for evaluation.*

[GitHub Repository for Skeleton Code](https://github.com/Rahul-Agrawal-09/MeMS-Skeleton-code)

