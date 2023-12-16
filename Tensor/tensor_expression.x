import CpuTensor as T
'''
 the script demonstrates basic tensor operations and graph-based execution, 
 which is typical in libraries used for numerical computations, scientific research, 
 and machine learning.
'''
t1 = tensor(3.1415,shape=[10,10])
t2 = 10+t1
t3 = tensor(2.718,shape=[10,10])
t4 = t2+t3
t_g = T.graph(t4)
print(t_g)
t_g.run()
print("end")
