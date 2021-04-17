import numpy as np
import os

def convert2numpy(mat):
	mat = mat.strip().split("\n")
	mat = [i.strip().split() for i in mat]
	for x in mat:
		for y in x:
			a, b = y.strip().split('.')
			assert len(b) == 12, "incorrect precision"
	return np.array(mat).astype(np.double)

def checkSingleCase(input_file, out_L, out_U):
	with open(input_file, "r") as f:
		A = f.read()
	with open(out_L, "r") as f:
		L = f.read()
	with open(out_U, "r") as f:
		U = f.read()	
	A, L, U = (convert2numpy(x) for x in (A,L,U))

	# forcing the matrices to be triangular
	L = np.tril(L)
	U = np.triu(U)

	assert U.shape == L.shape == A.shape, "invalid shape"

	A_dash = np.matmul(L, U)
	if np.allclose(A, A_dash, atol = 1e-3) and abs(np.linalg.det(U) - 1) < 1e-3:
		print("Works for {}".format(input_file))
	else:
		print("Invalid for {}".format(input_file))
		exit(0)

def num2string(x):
	s = str(x)
	l = s.split(".")[1]
	if len(l) > 12:
		l = l[:12]
	else:
		while len(l) < 12:
			l += "0"
	return s.split(".")[0] + "." + l

def generate_testcase(n, i):
	arr = np.random.rand(n, n)
	path = "testcases/input_" + str(n) + "_" + str(i) + ".txt"
	with open(path, "w") as f:
		s = ""
		for x in arr:
			for y in x:
				s += num2string((-10*np.random.random() + 5)*y) + " "
			s += "\n"
		f.write(s)
	return 

generate_tc = True
input_size = []
for i in range(1, 11):
	for j in range(5):
		input_size.append(i)
for i in range(100, 901, 200):
	for j in range(3):
		input_size.append(i)
for i in range(1000, 5001, 1000):
	for j in range(5):
		input_size.append(i)

if generate_tc:
	np.random.seed(0)
	for i in range(len(input_size)):
		x = input_size[i]
		generate_testcase(x, i-input_size.index(x))



