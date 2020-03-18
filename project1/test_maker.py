import random
import os
#maxStudents = 1000000
students = 10000
#students = int(random.random()*maxStudents)
grades = []
adjList = []
adj = 501000

#print(str(students)+','+str(adj))
s = ''
for i in range(students):
    s += str(random.randint(1, 20)) + '\n'
s = s[:-1]

for i in range(adj):
    t = tuple((random.randint(1, students), random.randint(1, students)))
    adjList += [t]
adjList = list(set(adjList))

print(str(students)+','+str(len(adjList)))
print(s)
for i in range(len(adjList)):
    print(adjList[i][0], adjList[i][1])

