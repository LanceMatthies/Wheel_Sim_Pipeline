#!/bin/bash

#SBATCH --nodes 1                         
#SBATCH --partition GPU-shared
#SBATCH --gpus=v100-32:2
#SBATCH --mail-type=ALL

#SBATCH --output output-%A_%a.out
#SBATCH --error error_log-%A_%a.err

# ----↓--- Params to Modify ------↓------

#SBATCH --time 00:01:00		# max job time, make sure it doesn't cut off your sim early!
#SBATCH --array=0			# array range corresponds with size of SLIPS vector

BATCH_NAME="automation_test_${SLURM_JOB_ID}"		# add a descriptive name here. Does not affect functionality
SIM_ENDTIME=0.2				# in seconds, how long to run the wheel for
SLIPS=(0.2)			# the range of slip values evaluated in the sim

BIN_DIR="${HOME}/moonranger_mobility/wheel_simulator/build"                       # Path to where you built the code
WHEEL_FILE="/jet/home/matthies/moonranger_mobility/meshes/wheel.obj"              # Path to wheel .obj
CLUMP_FILE="${HOME}/moonranger_mobility/terrain/GRC_3e5_Reduced_Footprint.csv"    # Path to terrain.csv
DATA_DIR="${PROJECT}"     # Path to storage drive

# ----↑---------------------------↑-----


echo "Loading modules..."

module load cuda/11.7.1
module load gcc/10.2.0

echo "Loaded modules."

echo "Navigating to target directory: ${BIN_DIR}"

cd ${BIN_DIR}

echo "Starting Sim"

job_id=$SLURM_JOB_ID
array_id=$SLURM_ARRAY_TASK_ID
echo "job ID ${job_id} array ID ${array_id}"

SLIP=${SLIPS[${array_id}]}
echo "Slip for array ID ${array_id} is ${SLIP}"

JSON_FILE="$1"
if [[ -z "$JSON_FILE" ]]; then
    echo "no JSON file provided"
    exit 1
fi

echo "Sim inputs: ${SLIP} ${SIM_ENDTIME} ${BATCH_NAME} ${WHEEL_FILE} ${CLUMP_FILE} ${DATA_DIR}"
./WheelSimulator $JSON_FILE $BATCH_NAME
echo "Sim complete"
