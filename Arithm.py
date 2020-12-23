import numpy as np
from mpi4py import MPI

comm = MPI.COMM_WORLD
t_start = MPI.Wtime()

N = 1000
my_N = N // 2

if comm.rank == 0:
    A = np.arange(N, dtype=np.float64)
else:
    A = np.empty(N, dtype=np.float64)

my_A = np.empty(my_N, dtype=np.float64)


comm.Scatter([A, MPI.DOUBLE], [my_A, MPI.DOUBLE])

print("After scatter:")
for r in range(comm.size):
    if comm.rank == r:
        print("[%d] %s" % (comm.rank, len(my_A)))
    comm.Barrier()


comm.Allgather([my_A, MPI.DOUBLE], [A, MPI.DOUBLE])

print("After allgather:")
for r in range(comm.size):
    if comm.rank == r:
        print("[%d] %s" % (comm.rank, len(A)))
    comm.Barrier()
t_diff = MPI.Wtime() - t_start
if comm.rank==0:
    print ("Elapsed time "+str(t_diff)+"s");
