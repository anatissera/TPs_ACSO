#!/bin/bash

# Directory containing the .x files
INPUT_DIR="inputs"

# Function to run simulator and capture output
run_simulator() {
    local sim=$1
    local input_file=$2
    local output_file=$3
    
    # Check if file exists and is not empty
    if [ ! -s "$input_file" ]; then
        echo "⚠️  Skipping $input_file: File is empty or does not exist"
        return 1
    fi
    
    # Run simulator and capture output with both register and memory dumps
    # Using proper memory ranges from shell.c
    echo -e "rdump\nmdump 0x10000000 0x10000020\nrun 1\nrdump\nmdump 0x10000000 0x10000020\nrun 1\nrdump\nmdump 0x10000000 0x10000020\n" | $sim $input_file > $output_file 2>&1
}

# Function to run full program
run_full_program() {
    local sim=$1
    local input_file=$2
    local output_file=$3
    
    # Check if file exists and is not empty
    if [ ! -s "$input_file" ]; then
        echo "⚠️  Skipping $input_file: File is empty or does not exist"
        return 1
    fi
    
    # Run until HLT
    echo -e "go\n" | $sim $input_file > $output_file 2>&1
}

# Create a temporary directory for outputs
mkdir -p temp_outputs

# Test categories based on utils folder structure
declare -A categories=(
    ["math"]="adds.x adds-subs.x add.x addis.x subis.s"
    ["logical"]="ands.x eor.x"
    ["branch"]="beq.x blt.x"
    ["data"]="movz.x sturb.x"
    ["shift"]="lsl_lsr.x"
    ["system"]="hlt.x"
)

# Process each category
for category in "${!categories[@]}"; do
    echo "================================================"
    echo "Testing $category instructions..."
    echo "================================================"
    
    # Process each file in the category
    for file in ${categories[$category]}; do
        if [ -f "$INPUT_DIR/$file" ]; then
            echo "Testing $file..."
            
            # Get filename without extension
            filename=$(basename "$file" .x)
            filename=$(basename "$filename" .s)
            
            # Test step by step
            echo "Testing step by step..."
            if run_simulator "./src/sim" "$INPUT_DIR/$file" "temp_outputs/${filename}_my_sim_step.txt"; then
                if run_simulator "./ref_sim_x86" "$INPUT_DIR/$file" "temp_outputs/${filename}_ref_sim_step.txt"; then
                    # Compare step by step outputs
                    if diff "temp_outputs/${filename}_my_sim_step.txt" "temp_outputs/${filename}_ref_sim_step.txt" > /dev/null; then
                        echo "✅ $filename: Step by step outputs match"
                    else
                        echo "❌ $filename: Step by step outputs differ"
                        echo "Differences:"
                        diff "temp_outputs/${filename}_my_sim_step.txt" "temp_outputs/${filename}_ref_sim_step.txt"
                    fi
                fi
            fi
            
            # Test full program execution
            echo "Testing full program execution..."
            if run_full_program "./src/sim" "$INPUT_DIR/$file" "temp_outputs/${filename}_my_sim_full.txt"; then
                if run_full_program "./ref_sim_x86" "$INPUT_DIR/$file" "temp_outputs/${filename}_ref_sim_full.txt"; then
                    # Compare full execution outputs
                    if diff "temp_outputs/${filename}_my_sim_full.txt" "temp_outputs/${filename}_ref_sim_full.txt" > /dev/null; then
                        echo "✅ $filename: Full execution outputs match"
                    else
                        echo "❌ $filename: Full execution outputs differ"
                        echo "Differences:"
                        diff "temp_outputs/${filename}_my_sim_full.txt" "temp_outputs/${filename}_ref_sim_full.txt"
                    fi
                fi
            fi
            
            echo "----------------------------------------"
        else
            echo "⚠️  Skipping $file: File does not exist"
        fi
    done
    echo ""
done

# Clean up
rm -rf temp_outputs 