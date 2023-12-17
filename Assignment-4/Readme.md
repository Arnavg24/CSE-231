# CSE-231 Operating Systems Section B
## Assignment 4: Concurrency and Synchronization Primitives

---

### Question 1: Dining Philosophers Variation

The dining philosophers problem (OSTEP Chapter 31, page 13) is a classic problem to demonstrate the concepts of deadlocks. The original setup contains five philosophers sitting on a round table, with a fork between each philosopher. Each philosopher can perform only one of two actions – eating and thinking. For eating, each philosopher requires 2 forks that are kept at their left and right sides. Allowing unrestricted access to each philosopher can lead to a deadlock.

Consider a modified version of the classic problem stated below:

Besides the 5 forks, there are now 2 bowls that are kept at the centre of the table. For eating, each of them now requires two forks and a bowl.

Model the above problem using threads as philosophers.

You must also ensure that there are no deadlocks in the code.

You are only allowed to use Mutexes (locks) and Conditional Variables for this question

Guideline:

Please structure your code to contain the following functions:

```c
void* philosopher(void* args)     // for running philosopher thread
void eating()                               // for entering eating state
void thinking()                            // for entering thinking state
Deliverables:

C code
Readme file explaining the following:

Why deadlocks can occur in the problem setup
How your proposed solution avoids deadlock.
Fairness of the solution i.e. for your implementation, which and how many of the 5 philosopher threads are able to eat, and a rough estimate of how often a philosopher is able to eat (if at all).
Question 2: Passengers and Car Ride Simulation
Imagine a situation where multiple passengers eagerly await their turn to take a ride in a car. This car has a limited capacity and can only set off when fully occupied, with a maximum of C passengers on board (where C is less than the total number of passengers). Passengers have the simple tasks of getting on and off the car, while the car itself must manage the loading, running, and unloading procedures. Passengers are allowed to board only when the car has completed the loading process, and the car can commence its journey once it has reached its maximum passenger capacity. Passengers can disembark from the car only after it has completed its unloading process.

Simulate the above by modeling the car and the passengers as threads. Take the total number of passengers and capacity as input from the user.

c
Copy code
void* car(void* args)                   // car thread
void* passenger(void* args)         // passenger thread
void load()                                    // loading car with passengers
void unload()                                // unloading passengers
void board()                                  // passenger boards car
void offboard()                         // passenger gets off car
Deliverables:

C code
Writeup explaining code logic and how you avoid concurrency bugs in code

Question 3: River Bridge Car Crossing
Consider the following problem:

There is a thin bridge between two sides of a river. Due to its size, only one car can travel from the left side of the river to the right and vice versa. The number of cars that can travel on the bridge simultaneously is 5. If 2 cars from opposite sides travel on the bridge simultaneously, they will not be able to cross, and travel will stop.

Modeling each car as a thread, write a program such that all cars from the left and the right side are able to cross without violating the above constraints (the number of cars on the left and right is to be taken as input from the user).

c
Copy code
void* left(void* args)                        // cars on the left
void* right(void* args)                // cars on the right
void passing(int dir)                        // car from some direction is traveling on the bridge
Deliverables:

C code
Writeup explaining code logic and how you avoid concurrency bugs in code
