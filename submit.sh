#!/bin/bash
#
#SBATCH --cpus-per-task=4
#SBATCH --time=10:00
#SBATCH --mem=5G

srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_correctness --n_producers 1 --n_consumers 1 --input_file /scratch/assignment3/inputs/rand_100M
srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_correctness --n_producers 2 --n_consumers 2 --input_file /scratch/assignment3/inputs/rand_100M
srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_correctness --n_producers 3 --n_consumers 3 --input_file /scratch/assignment3/inputs/rand_100M
srun /home/$USER/sfuhome/CMPT431/A3/./one_lock_queue_correctness --n_producers 4 --n_consumers 4 --input_file /scratch/assignment3/inputs/rand_100M