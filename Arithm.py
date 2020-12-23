import numpy as np
from mpi4py import MPI

comm = MPI.COMM_WORLD
t_start = MPI.Wtime()

N = 10
my_N = N // 2

if comm.rank == 0:
    A = np.arange(N, dtype=np.uint64)
else:
    A = np.empty(N, dtype=np.uint64)

my_A = np.empty(my_N, dtype=np.uint64)


comm.Scatter([A, MPI.UNSIGNED_LONG_LONG], [my_A, MPI.UNSIGNED_LONG_LONG])

print("After scatter:")
for r in range(comm.size):
    if comm.rank == r:
        print("[%d] %s" % (comm.rank, len(my_A)))
    comm.Barrier()


comm.Allgather([my_A, MPI.UNSIGNED_LONG_LONG], [A, MPI.UNSIGNED_LONG_LONG])

print("After allgather:")
for r in range(comm.size):
    if comm.rank == r:
        print("[%d] %s" % (comm.rank, len(A)))
    comm.Barrier()
t_diff = MPI.Wtime() - t_start
if comm.rank==0:
    print ("Elapsed time "+str(t_diff)+"s");
