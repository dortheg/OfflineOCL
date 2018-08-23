import numpy as np 
import matplotlib.pyplot as plt 


x = np.zeros((12))
y = np.zeros((12))

x[0] = 1009.75 #is actually 511!
x[1] = 1662.96
x[2] = 2003.17
x[3] = 2506.69 #is actually 1273!
x[4] = 3146.29
x[5] = 3500.11
x[6] = 3976.41
x[7] = 4357.45
x[8] = 4752.0
x[9] = 5282.0
x[10] = 5541.0
x[11] = 5895.0


x1 = x[0]
y1 = 511.0

x2 = x[3]
y2 = 1273.0


a = (y2-y1)/float(x2-x1)

def y_func(x):
	return a*x - a*x1 + y1


print("a: ", a, " b: ", -a*x1+y1)

y = y_func(x)

print(y_func(9576))

#print(y)

