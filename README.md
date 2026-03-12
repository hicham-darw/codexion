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

## What Is a multithreading?
Multithreading is a CPU technique allowing a single process to split into multiple, independent execution units (threads) that run concurrently, enhancing performance and responsiveness. Threads share resources like memory, making them lightweight compared to separate processes. It is commonly used in web servers, gaming, and applications to handle tasks simultaneously.

## Why Use Multithreading?
With the introduction of multiple cores, multithreading has become extremely important in terms of the efficiency of your application. With multiple threads and a single core, your application would have to transition back and forth to give the illusion of multitasking.
With multiple cores, your application can take advantage of the underlying hardware to run individual threads through a dedicated core, thus making your application more responsive and efficient. Multithreading basically allows you to take full advantage of your CPU and the multiple cores, so you don’t have untapped processing power with idle cores.
- Higher throughput
- Responsive applications that give the illusion of multitasking.
- Efficient utilization of resources. Thread creation is light-weight in comparison to spawning a brand new - process and for web servers that use threads instead of creating a new process when fielding web requests, - consume far fewer resources