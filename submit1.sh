#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=10:00
#SBATCH --mem=5G

srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_throughput --n_producers 5 --n_consumers 5 --seconds 10 --init_allocator 100000000
srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_throughput --n_producers 6 --n_consumers 6 --seconds 10 --init_allocator 100000000
srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_throughput --n_producers 6 --n_consumers 7 --seconds 10 --init_allocator 100000000
srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_throughput --n_producers 8 --n_consumers 8 --seconds 10 --init_allocator 100000000