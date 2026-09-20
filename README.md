# Dining-Philosiphers
Demonstrating the 'Dining Philosiphers' Problem in real time and explaining how it relates to Operating Systems

The Dining Philosihpers problem is a classic computer science synchronization problem which seeks to demonstrate the the sharing of resources, the occurance of deadlock and concurrency within a system where multiple running processes each require limited resources.
A simplistic explanation of the problem is as follows:
<br>
A group of philosiphers sit around a table, with a limited number of chopsticks and a bowl of rice. Each of the philosiphers can be either **THINKING**, **HUNGRY**, or **EATING**. Not all philosiphers may eat at once due to the limited number of chopsticks on the table. So in order to assure the table operates safely there must be a system to ensure consistency and concurrency among them so that they can safely transition between their various states of **THINKING**, **HUNGRY**, or **EATING** without causing issues such as deadlock.
<br>
A state of deadlock is achieved when two or more processes are unable to be completed due to resources required for completion being unavailable. In this context, deadlock refers to a state in which a philosipher is left perpetually waiting for a chopstick to eat that is currently being held by another philosipher.

## Solution
The proposed solution for the dining philosiphers problem is to build