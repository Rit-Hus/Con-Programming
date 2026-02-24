The solution is implemented using Java sockets. 
The algorithm follows a centralized client-server model where the Teacher (server) takes in two student sockets at a time, pairs them by swapping their indices, 
and then closes the connections to open up for the next pair. The Teacher class is initialized with the total number of students. 
When the number of students left to be processed is one (in the case of an odd total), the Teacher pairs that student with their own index. 
Each student process connects, waits for the partner's index from the Teacher, and prints the result.