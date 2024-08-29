<div align="center">
    <h1>Dining Philosophers Problem</h1>
</div>

* [Explaining the Problem](#Explaining-the-Problem)
* [The-Challenge](#The-Challenge)
* [Key-Concepts](#Key-Concepts)
* [Building-and-Running](#Building-and-Running)
* [Example-tests](#Example-tests)
* [Conclusion](#Conclusion)

## Explaining the Problem
The Dining Philosophers Problem is a classic synchronization problem involving philosophers sitting at a roung table with a fork placed between each pair of abjacent philosophers. Each philosopher must alternately think, eat, and sleep. 

>**However**    
> A philosopher can only eat when they ahve both left and right forks. After eating, they put down the forks and start thinking again.

The problem is to design a program that allows philosophers to never be starved and avoid deadlock while trying to pick up the two forks adjacent to them to eat.

## The Challenge
At this point I am sure you're thinking -"What the heck?". 
But let me explain...

Considering that a philosopher can only eat when it holds its two adjacent forks, then it makes sense to think that 1 philosopher will die of starvation if its the only one in the table, right?

Now, if we gave 2 philosophers in the table one of them is going to look the other one eating while waiting for its turn, then when the forks are available its its turn to eat, and so on. 

This is perfect, but what happends if both of them takes it own fork at the same time? --> DEADLOCK

>**Deadlock**   
>Occurs when 2 or more philosophers are waiting on each other to release its forks, and thus none of them can proceed and they died of starvation.

## Key Concepts
This project is about learning what are threads and mutexes and how to apply them with this project.

Let's understand what are threads and mutexes first:
 - **Threads:** Theads are the smallest sequence of programmed instructions that can be managed independently. They are used to improve the performance of operating systems and application by allowing multiple tasks to be executed concurrently within the same process.

 - **Mutexes:** Mutexes are needed to prevent deadlocks. In other words, they are present to prevent simultaneous usage to shared resources (forks in this case) by multiple threads (philosophers).

 ## Building and Running
 Take a look at this project.

 1. Git clone:
    ```
    $ git clone https://github.com/MemoCSales/42_philosophers.git
    ```

 2. Build and run:

    ```
    $ cd 42_philosophers/philo/
    $ make
    $ ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
    ```

## Example tests
 1. The philosopher should not eat and should die
    ````
    $ ./philo 1 800 200 200
    ````
 2. No philosopher should die
    ````
    $ ./philo 5 800 200 200
    ````
 3. One philosopher should die
    ````
    $ ./philo 4 310 200 100
    ````

## Conclusion
This project gave me a practical implementation of the Dinning Philosopher Problem by understanding the usage of threads and mutexes for synchronization in C. It addresses the challenges of deadlock and starvation, ensuring that all philosophers can eat and think without interference.