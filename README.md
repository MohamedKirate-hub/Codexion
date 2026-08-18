# Codexion

*This project has been created as part of the 42 curriculum by mkirate*

## Description

**Codexion** is a C concurrency project using POSIX threads.

The simulation contains multiple coders, represented by threads, that share a limited number of dongles. Each coder needs **two dongles at the same time** to compile.

The main goals are:

* Manage threads safely.
* Protect shared resources.
* Prevent deadlocks and starvation.
* Implement FIFO and EDF scheduling.
* Handle dongle cooldowns.
* Detect coder burnout.
* Synchronize log output.
* Stop the simulation correctly.

---

## How It Works

Coders are arranged in a circle, with one dongle between each pair.

Each coder repeatedly does:

```text
Compile → Debug → Refactor → Compile → ...
```

To compile, a coder must acquire both neighboring dongles.

The simulation stops when:

* Every coder completes the required number of compilations, or
* A coder burns out.

---

## Burnout

A coder must start its next compilation before its `time_to_burnout` deadline.

The deadline is based on:

```text
last_compile_start + time_to_burnout
```

A monitor thread checks these deadlines and stops the simulation when a coder burns out.

The burnout message must be printed within approximately **10 ms** of the actual burnout time.

---

## Arguments

```text
./codexion number_of_coders time_to_burnout time_to_compile \
           time_to_debug time_to_refactor \
           number_of_compiles_required dongle_cooldown scheduler
```

| Argument                      | Description                                 |
| ----------------------------- | ------------------------------------------- |
| `number_of_coders`            | Number of coders and dongles                |
| `time_to_burnout`             | Maximum time before the next compilation    |
| `time_to_compile`             | Compilation time in ms                      |
| `time_to_debug`               | Debugging time in ms                        |
| `time_to_refactor`            | Refactoring time in ms                      |
| `number_of_compiles_required` | Required compilations per coder             |
| `dongle_cooldown`             | Time before a released dongle can be reused |
| `scheduler`                   | `fifo` or `edf`                             |

All time values are in milliseconds.

---

## Scheduling

### FIFO

**First In, First Out.**

The coder that requested the resource first gets priority.

### EDF

**Earliest Deadline First.**

The coder with the earliest burnout deadline gets priority.

For example:

```text
Coder 1 → 2500 ms
Coder 2 → 1800 ms
Coder 3 → 2200 ms
```

EDF chooses:

```text
Coder 2 → Coder 3 → Coder 1
```

---

## Priority Queue

The scheduler uses a custom **heap-based priority queue**.

* FIFO uses request order.
* EDF uses burnout deadlines.

This allows the scheduler to efficiently select the next coder.

---

## Thread Architecture

Each coder has its own POSIX thread, and a separate monitor thread checks burnout.

```text
              Main Thread
                   |
        +----------+----------+
        |          |          |
     Coder 1    Coder 2    Coder N
        |          |          |
        +----------+----------+
                   |
              Dongles
                   |
               Scheduler
                   |
               Monitor
```

The main thread creates the threads and waits for them to finish.

---

## Synchronization

### Mutexes

`pthread_mutex_t` protects shared resources and state.

For example, a dongle must never be acquired by two coders simultaneously.

### Condition Variables

`pthread_cond_t` allows coders to wait efficiently until a resource or state becomes available, instead of continuously checking it.

---

## Problems Handled

### Deadlock

Coders can easily create a circular wait when each holds one dongle and waits for another. The resource management system must prevent this situation.

### Starvation

A coder must not wait forever while other coders repeatedly get access to the resources. FIFO and EDF provide different fairness strategies.

### Dongle Cooldown

After a dongle is released, it remains unavailable for `dongle_cooldown` milliseconds.

### Burnout Detection

The monitor thread checks coder deadlines and stops the simulation when necessary.

### Log Synchronization

A logging mutex prevents messages from different threads from being mixed together.

---

## Example

```bash
./codexion 5 2000 200 200 200 10 0 fifo
```

This creates:

* 5 coders
* 5 dongles
* 2000 ms burnout limit
* 200 ms compile/debug/refactor times
* 10 required compilations
* No cooldown
* FIFO scheduling

---

## Output

The format is:

```text
timestamp_in_ms coder_id action
```

Possible actions include:

```text
has taken a dongle
is compiling
is debugging
is refactoring
burned out
```

Example:

```text
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
```

Each message must be printed as a complete line.

---

## Compilation

```bash
make
```

The project is compiled with:

```text
-Wall -Wextra -Werror -pthread
```

Available commands:

```bash
make
make clean
make fclean
make re
```

---

## Edge Cases

The program should correctly handle:

* One coder.
* Very small burnout times.
* Large dongle cooldowns.
* Many competing coders.
* Invalid arguments.
* FIFO and EDF scheduling.
* Coders unable to acquire both dongles before their deadline.
* Successful completion by all coders.

Example:

```bash
./codexion 1 800 200 200 200 10 0 fifo
```

With one coder, there is only one dongle, but compilation requires two, so the coder eventually burns out.

---

## Memory Management

All allocated memory must be freed before exiting, including:

* Coder structures
* Dongle structures
* Heap and scheduler data
* Priority queue storage
* Other allocated resources

Mutexes and condition variables must also be properly destroyed after all threads have stopped using them.

---

# Project Structure

The project is organized into several files, with each file having a specific responsibility.

```text
.
├── coders/
│   ├── cleaner.c
│   ├── coders.c
│   ├── coder_utils.c
│   ├── codexion.h
│   ├── data_race.c
│   ├── dongle_utils.c
│   ├── error.c
│   ├── ft_atoi.c
│   ├── init.c
│   ├── main.c
│   ├── min_heap.c
│   ├── min_heap_utils.c
│   ├── monitor.c
│   ├── parser.c
│   ├── request_dongles.c
│   ├── setup_coders.c
│   ├── setup_dongles.c
│   ├── take_dongle.c
│   └── utils.c
├── Makefile
└── README.md
```

## Source Files

| File                | Responsibility                                                                              |
| ------------------- | ------------------------------------------------------------------------------------------- |
| `main.c`            | Entry point of the program. Starts the initialization and simulation.                       |
| `codexion.h`        | Main header file containing shared structures, types, constants, and function declarations. |
| `parser.c`          | Parses and validates the program arguments.                                                 |
| `ft_atoi.c`         | Converts numeric strings into integers.                                                     |
| `init.c`            | Initializes the main simulation data and shared information.                                |
| `setup_coders.c`    | Creates and initializes the coder structures and their relationships.                       |
| `setup_dongles.c`   | Creates and initializes the dongles and their synchronization objects.                      |
| `coders.c`          | Contains the main routine executed by each coder thread.                                    |
| `coder_utils.c`     | Helper functions used by the coder routine.                                                 |
| `request_dongles.c` | Handles coder requests for the required dongles and interacts with the scheduling system.   |
| `take_dongle.c`     | Contains the logic used to acquire and release dongles safely.                              |
| `dongle_utils.c`    | Helper functions related to dongle management.                                              |
| `min_heap.c`        | Implements the min-heap used by the scheduling system.                                      |
| `min_heap_utils.c`  | Helper functions for managing the min-heap.                                                 |
| `monitor.c`         | Contains the monitor thread responsible for checking coder deadlines and detecting burnout. |
| `data_race.c`       | Contains synchronization logic used to protect shared data and prevent data races.          |
| `cleaner.c`         | Frees allocated memory and destroys mutexes and other synchronization resources.            |
| `error.c`           | Handles error messages and error reporting.                                                 |
| `utils.c`           | General utility functions used throughout the project.                                      |

---

## What I Learned

This project provided practical experience with:

* POSIX threads
* Mutexes
* Condition variables
* Race conditions and data races
* Deadlocks and starvation
* Resource management
* FIFO and EDF scheduling
* Priority queues and heaps
* Thread monitoring
* Time management
* Thread-safe logging
* Memory management

The main lesson is that creating threads is only the beginning. The real challenge is coordinating them safely when they share resources and have strict timing requirements.

---

## Resources

* [Thread Control Block (TCB)](https://www.geeksforgeeks.org/operating-systems/thread-control-block-in-operating-system/)
  Helped me understand how threads are represented and managed by the operating system.

* [Process States](https://www.youtube.com/watch?v=jZ_6PXoaoxo)
  Helped me understand the different states of a process and how it moves between them.

* **The Linux Programming Interface** — Michael Kerrisk
  Helped me understand Linux system programming and how to use POSIX threads in C.

* [Min-Heap](https://www.youtube.com/watch?v=HqPJF2L5h9U&t=1750s)
  Helped me understand the min-heap data structure and how it can be implemented and used for priority-based scheduling.



## AI Usage

AI was used as a learning and review tool during the project.

It helped me:

* Understand some concurrency and synchronization concepts.
* Create and improve the `README.md` file.

---

## Conclusion

Codexion demonstrates how threads, synchronization, scheduling, and resource management work together in a real C program.

The main challenge is allowing multiple coders to work concurrently while ensuring that:

* Dongles are never shared incorrectly.
* Starvation is controlled.
* Deadlocks are avoided.
* Cooldowns are respected.
* Burnout is detected accurately.
* Logs remain consistent.
* Resources are properly cleaned up.
