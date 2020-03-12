import random
import os
maxStudents = 50
students = 0
adj = 0
students = int(random.random()*maxStudents)
grades = []
adjList = []
adj = int(random.random()*maxStudents)

print(str(students)+','+str(adj))
for i in range(students):
    print(random.randint(1, 21))

for i in range(adj):
    t = tuple((random.randint(0, students), random.randint(0, students)))
    adjList += [t]
adjList = list(set(adjList))
for i in range(len(adjList)):
    print(adjList[i][0], adjList[i][1])
