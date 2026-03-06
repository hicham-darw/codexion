# Codexion
Codexion is a concurrency and synchronization challenge where you must orchestrate multiple coder threads competing for limited hardware resources in a simulated environment. The simulation takes place in a circular co-working hub featuring a shared Quantum Compiler in the cente

- The Resource Conflict: There are as many USB dongles as coders, but a coder requires two dongles simultaneously (one in each hand) to start compiling. Because one dongle sits between each pair of coders, they must share these resources with their immediate neighbors, leading to potential deadlocks and competition

- The Coder Lifecycle: Coder threads rotate through three main states: compiling, debugging, and refactoring. After refactoring, a coder immediately tries to acquire dongles to compile again

- Burnout Deadlines: Every coder has a strict time_to_burnout limit. If they do not start a new compilation within this timeframe (measured from the start of their last compilation or the start of the simulation), they "burn out," and the simulation ends

- Mandatory Arbitration: When multiple coders request the same dongle, you must use a priority queue (heap) to decide who gets it based on a specified scheduler:
1- FIFO (First In, First Out): Requests are served in arrival order.
2- EDF (Earliest Deadline First): The coder closest to their burnout deadline is served first

- Technical Constraints: The project must be written in C using POSIX threads and mutexes. Global variables are strictly forbidden, and you must ensure log serialization so that messages from different threads do not interleave on the same line

- Precision Monitoring: A separate monitor thread is required to oversee the simulation and ensure that any burnout is logged within 10 ms of the actual event

## What Is A Thread?
a thread is defined as independent stream of instructions that can be scheduled to run as such by the operating system,
for software developer is a procedure that runs independently!

## What Are Pthread
