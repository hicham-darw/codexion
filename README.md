*This project has been created as part of the 42 curriculum by hel-hamo.*

# Codexion

Codexion is a 42 concurrency project where multiple coder threads compete for shared dongles and must be scheduled safely. The project focuses on correct thread synchronization, fairness under contention, and accurate burnout monitoring while keeping logs deterministic and readable.

## Description
Codexion is a concurrent systems project inspired by resource-sharing problems like Dining Philosophers. It simulates multiple coder threads that repeatedly:

1. request access to two shared dongles,
2. compile,
3. debug,
4. refactor,
5. then request resources again.

The goal is to build a robust multi-threaded simulation in C (POSIX threads) that stays correct under contention:

- no deadlocks,
- no data races on shared state,
- deterministic arbitration under FIFO or EDF scheduling,
- correct cooldown handling for reused dongles,
- precise burnout detection,
- serialized logging output.

Each coder has a burnout deadline (`time_to_burnout`). If a coder cannot start compiling before that deadline, the monitor stops the simulation.

## Instructions

### Requirements
- Linux or Unix-like environment
- `cc` compiler
- `make`
- POSIX threads (`-pthread`)

### Compilation
```bash
make
make all
```

### Cleanup
```bash
make clean
make fclean
make re
```

### Execution
```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```


Argument summary:
- `number_of_coders`: total coder threads in the simulation
- `time_to_burnout`: max time allowed without starting a new compile
- `time_to_compile`: time spent in compile phase
- `time_to_debug`: time spent in debug phase
- `time_to_refactor`: time spent in refactor phase
- `number_of_compiles_required`: per-coder compile target before normal stop
- `dongle_cooldown`: lockout period after dongles are released
- `scheduler`: arbitration policy (`fifo` or `edf`)

Example:
```bash
./codexion 5 800 200 200 200 7 50 edf
```

### Troubleshooting
- If compilation fails, run `make re` to rebuild all objects.
- If execution prints usage, verify you passed exactly 8 arguments after `./codexion`.s

## Blocking cases handled

### Deadlock prevention
- Circular wait is broken by asymmetric lock ordering in dongle acquisition.
- Odd coder IDs lock left then right; even coder IDs lock right then left.
- This prevents all threads from holding one dongle and waiting forever for the other.

### Coffman conditions perspective
- Mutual exclusion still exists (dongles are protected resources).
- Hold-and-wait is reduced by centralized granting via manager + heap arbitration.
- Circular wait is explicitly prevented by opposite lock orders.
- Because at least one Coffman condition is broken, deadlock is prevented in practice.

### Starvation prevention
- Waiting coders are inserted into a shared priority heap.
- `fifo` mode serves by arrival time.
- `edf` mode serves by earliest burnout deadline.
- A dedicated manager thread continuously pops eligible coders and signals them to continue, which avoids indefinite passive spinning.

### Cooldown handling
- After a compile, released dongles receive `cooldown_time = now + dongle_cooldown`.
- A coder can only take both dongles when both cooldown timestamps have expired.
- This models hardware settling time and prevents immediate re-grab races.

### Precise burnout detection
- A monitor thread polls all coders at short intervals (`usleep(500)`).
- Burnout is checked against `last_compile` and `time_to_burnout`.
- Once burnout is detected, stop is broadcast and the event is logged.
- This keeps detection latency near the required precision window.

### Log serialization
- Every log line is guarded by a global print mutex.
- This guarantees that concurrent threads cannot interleave partial messages.
- Output remains line-consistent and auditable under high contention.

## Thread synchronization mechanisms

### pthread_mutex_t usage
- `mutex_dongle`: protects dongle ownership and cooldown timestamps.
- `mutex_coder`: protects per-coder state (`can_compile`, `is_compiling`, counters, deadlines).
- `mutex_heap`: protects shared scheduler heap operations (insert/pop/heapify).
- `mutex_stop`: protects global stop flag updates/reads.
- `mutex_print`: serializes console output.

Race-condition prevention examples:
- Two coders cannot mark the same dongle as taken at the same time because both dongles are locked before ownership changes.
- Monitor reads and coder updates of timing/status fields are synchronized with `mutex_coder`.
- Shared queue ordering remains consistent because all heap mutations are under `mutex_heap`.

### pthread_cond_t usage
- Each coder owns `cond_coder`.
- A coder blocks in `waiting_to_compile` until manager grants permission (`can_compile = 1`) and broadcasts the condition.
- This removes busy waiting and allows explicit wake-up when resources become available.

### Custom event implementation (manager/heap signaling model)
- The project implements an event-like flow without external event libraries:
1. coder publishes a request event by inserting itself into the heap,
2. manager consumes events from the heap and tries to allocate dongles,
3. manager emits a wake-up event by toggling `can_compile` and signaling `cond_coder`,
4. coder resumes and compiles.

This design provides thread-safe communication between coders and the monitor/manager pipeline while keeping shared-state ownership explicit.

## Resources

### Classic references
- POSIX Threads Programming (LLNL): https://hpc-tutorials.llnl.gov/posix/
- Linux man pages: `pthread_create(3)`, `pthread_mutex_lock(3)`, `pthread_cond_wait(3)`
- Operating Systems: Three Easy Pieces (Concurrency chapters): https://pages.cs.wisc.edu/~remzi/OSTEP/
- The Little Book of Semaphores (Allen B. Downey): https://greenteapress.com/wp/semaphores/
