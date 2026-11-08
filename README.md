# Philosophers

A C implementation of the Dining Philosophers problem using POSIX threads and mutexes. Philosophers alternate between eating, sleeping, and thinking while sharing forks. The simulation monitors meals and reports when a philosopher dies.

## Requirements

- A C compiler (`cc`)
- `make`
- POSIX threads

## Build

The source code and Makefile are in the `philo/` directory:

```sh
cd philo
make
```

This creates the `philo` executable. To remove object files and the executable:

```sh
make clean
make fclean
```

## Usage

Run the program from the `philo/` directory:

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_per_philosopher]
```

All time values are in milliseconds. The optional final argument sets how many times each philosopher must eat before the simulation ends.

### Example

```sh
./philo 5 800 200 200
```

Run five philosophers with an 800 ms time-to-die, 200 ms time-to-eat, and 200 ms time-to-sleep.

```sh
./philo 5 800 200 200 7
```

The same simulation, stopping after each philosopher has eaten seven times.

## Output

Each status line includes the elapsed time in milliseconds, the philosopher’s number, and an event:

```text
timestamp_in_ms philosopher_number is eating
timestamp_in_ms philosopher_number is sleeping
timestamp_in_ms philosopher_number is thinking
timestamp_in_ms philosopher_number has taken a fork
timestamp_in_ms philosopher_number died
```
