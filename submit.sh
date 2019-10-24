#!/bin/bash
#
#SBATCH --cpus-per-task=4
#SBATCH --time=10:00
#SBATCH --mem=5G

srun /home/$USER/sfuhome/CMPT431/A3/./non_blocking_queue_correctness --n_producers 5 --n_consumers 4 --input_file /scratch/assignment3/inputs/rand_10M
#srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_throughput --n_producers 3 --n_consumers 6 --seconds 5 --init_allocator 100000000