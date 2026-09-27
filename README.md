# Dining-Philosiphers
Demonstrating the 'Dining Philosiphers' Problem in real time and explaining how it relates to Operating Systems

The Dining Philosihpers problem is a classic computer science synchronization problem which seeks to demonstrate the the sharing of resources, the occurance of deadlock and concurrency within a system where multiple running processes each require limited resources.
A simplistic explanation of the problem is as follows:
<br>
A group of philosiphers sit around a table, with a limited number of chopsticks/Forks and a bowl of rice. Each of the philosiphers can be either **THINKING**, **HUNGRY**, or **EATING**. Not all philosiphers may eat at once due to the limited number of chopsticks on the table. So in order to assure the table operates safely there must be a system to ensure consistency and concurrency among them so that they can safely transition between their various states of **THINKING**, **HUNGRY**, or **EATING** without causing issues such as deadlock. In order to eat a philosipher must have 2 chopsticks (one from either side of them) and may only pick up either chopstick at once. 
<br>
A state of deadlock is achieved when two or more processes are unable to be completed due to resources required for completion being unavailable. In this context, deadlock refers to a state in which a philosipher is left perpetually waiting for a chopstick to eat that is currently being held by another philosipher.

## Solution
When solving the dining philosiphers problem we must keep in mind a few properties which mark success;
1. No Two philosiphers may eat at the same time - [See the Mutual Exclusion Principle](https://www.geeksforgeeks.org/operating-systems/mutual-exclusion-in-synchronization/)
2. Each Philosipher is guaranteed to eat in a finite amount of time
3. When a few philosiphers are waiting then nobody gets to eat for a while - Avoiding [Starvation](https://www.geeksforgeeks.org/operating-systems/starvation-and-aging-in-operating-systems/)

### Using Semaphors
An easy solution could be the usage of a [semaphore](https://www.geeksforgeeks.org/operating-systems/semaphores-in-process-synchronization/) to control different processes and ensure there are no race conditions within the application. However this could easily lead to a situation of deadlock in which all Philosiphers pick up their own left fork before any of them attempt to pick up their right fork which would leave all philosiphers waiting for their fork and none of them will execute anything. 
An easy solution to this problem could be to limit the number of philosiphers/processes that can be active at any point in time using a semaphore. This approach will eventually ensure that a single philsipher can acquire both their chopsticks and execute accordingly which will ultimately prevent and avoid deadlock.

## Chandy/Misra Solution
A desireable solution would be to implement a system in which the first 5 philosiphers effectively act as normal and execute their original solution, this time with the final philosipher waiting for their right then their left fork as opposed to their left then right.
This approach will actively ensure the processes remain free from Starvation, Deadlock while allowing a larger degree of concurrency among them. 

## Using Monitors
Believe it or not the above solution can be further enhanced by the usage of [Monitors](https://www.geeksforgeeks.org/operating-systems/monitors-in-process-synchronization/). These are higher level tools used for process synchronization and thread management. It ensures things such as automatic mutual exclusion and can be thought of similarly to a class as it manages multiple variables or method calls at once as opposed to calling them manually when dealing with semaphores. In the java language Monitors are implemented through using classes and synchronized methods.

In solving this problem the Monitor will manage an array of forks which will track the number of available forks for each philosipher. The operation to take a fork will wait on the condition variable that 2 forks are available, after which a fork is picked up and the number of available forks for its neighbour is decremented, it then leaves the monitor. After the philosipher finishes eating the operation to release a fork is called which updates the array of forks before checking to see if the newly available forks makes it possible to signal other philosiphers. 