#!/bin/bash

# Directory containing the .x files
INPUT_DIR="inputs"

# Function to run simulator and capture output
run_simulator() {
    local sim=$1
    local input_file=$2
    local output_file=$3
    
    # Run simulator and capture output
    echo -e "rdump\nrun 1\nrdump\nrun 1\nrdump\n" | $sim $input_file > $output_file
}

# Create a temporary directory for outputs
mkdir -p temp_outputs

# Process each .x file
for file in $INPUT_DIR/*.x; do
    if [ -f "$file" ]; then
        echo "Testing $file..."
        
        # Get filename without extension
        filename=$(basename "$file" .x)
        
        # Run both simulators
        run_simulator "./src/sim" "$file" "temp_outputs/${filename}_my_sim.txt"
        run_simulator "./ref_sim_x86" "$file" "temp_outputs/${filename}_ref_sim.txt"
        
        # Compare outputs
        if diff "temp_outputs/${filename}_my_sim.txt" "temp_outputs/${filename}_ref_sim.txt" > /dev/null; then
            echo "✅ $filename: Outputs match"
        else
            echo "❌ $filename: Outputs differ"
            echo "Differences:"
            diff "temp_outputs/${filename}_my_sim.txt" "temp_outputs/${filename}_ref_sim.txt"
        fi
        echo "----------------------------------------"
    fi
done

# Clean up
rm -rf temp_outputs 