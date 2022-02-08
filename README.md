# Semaphore

Using semaphores to solve producer-consumer problem.

## Program works as following:

N mCounter threads count independent incoming messages in a system and another
thread.
mMonitor gets the count of threads at time intervals of size t1, and then resets the
counter to 0. 
The mMonitor then places this value in a buffer of size b, and a mCollector thread
reads the values from the buffer.
Any thread will have to wait if the counter is being locked by any other thread. Also, the
mMonitor and mCollector threads will not be able to access the buffer at the same time
or to add another entry if the buffer is full.
Assume that the messages come randomly to the system, this can be realized if the
mCounter threads sleep for random times, and their activation (sleep time ends) corresponds to an
email arrival. Similarly, the mMonitor and mCollector will be activated at random time intervals.

The following figure represents the shared resources (the integer and the buffer) and
which threads are interacting with each resource:

![image](https://user-images.githubusercontent.com/63178601/153061411-334792e4-7028-4f8e-a67c-2c9c68f5f03b.png)

