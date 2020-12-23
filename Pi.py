import numpy as np
import math as m
from mpi4py import MPI
comm = MPI.COMM_WORLD

comm.Barrier()
t_start = MPI.Wtime()

data = np.zeros(2)
for i in range(comm.rank, 10000000, comm.size):
    data[0]=float("1E-14")*i*i
    data[1]+=1.0/(1.0+data[0])
#for i in range(comm.rank, len(data), comm.size):
#    data[i] = i+1

#print ('[%i]'%comm.rank, data)
comm.Barrier()

if comm.rank==0:
    totals = np.zeros_like(data)
else:
    totals = []

comm.Reduce(
    [data, MPI.DOUBLE],
    [totals, MPI.DOUBLE],
    op = MPI.SUM,
    root = 0
)
if len(totals)!=0:
    #print ('[%i]'%comm.rank, totals)
    print("Calculated Pi =",4*totals[1]*float("1E-7"))
    print("Real       Pi =",m.pi)

comm.Barrier()
t_diff = MPI.Wtime() - t_start
if comm.rank==0:
    print ("Elapsed time "+str(t_diff)+"s");
