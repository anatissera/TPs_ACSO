#!/bin/bash

INPUT_DIR="inputs/bytecodes"
LOG_FILE="resultados.txt"
mkdir -p temp_outputs
echo "🧪 Resultados de las comparaciones:" > $LOG_FILE

declare -A categories=(
    ["math"]="adds.x adds-subs.x add.x addis.x subis.s"
    ["logical"]="ands.x eor.x"
    ["branch"]="beq.x blt.x"
    ["data"]="movz.x sturb.x"
    ["shift"]="lsl_lsr.x"
    ["system"]="hlt.x"
)

run_simulator() {
    local sim=$1
    local input_file=$2
    local output_file=$3

    if [ ! -s "$input_file" ]; then
        echo "⚠️  $input_file vacío o inexistente" >> $LOG_FILE
        return 1
    fi

    echo -e "rdump\nmdump 0x10000000 0x10000020\nrun 1\nrdump\nmdump 0x10000000 0x10000020\nrun 1\nrdump\nmdump 0x10000000 0x10000020\n" | $sim $input_file > $output_file 2>&1
}

run_full_program() {
    local sim=$1
    local input_file=$2
    local output_file=$3

    if [ ! -s "$input_file" ]; then
        echo "⚠️  $input_file vacío o inexistente" >> $LOG_FILE
        return 1
    fi

    echo -e "go\n" | $sim $input_file > $output_file 2>&1
}

for category in "${!categories[@]}"; do
    echo "================================================" >> $LOG_FILE
    echo "Testing $category instructions..." >> $LOG_FILE
    echo "================================================" >> $LOG_FILE

    for file in ${categories[$category]}; do
        if [ -f "$INPUT_DIR/$file" ]; then
            filename=$(basename "$file" .x)
            filename=$(basename "$filename" .s)

            echo "🔹 Testing $file..." >> $LOG_FILE

            if run_simulator "./src/sim" "$INPUT_DIR/$file" "temp_outputs/${filename}_my_sim_step.txt"; then
                if run_simulator "./ref_sim_x86" "$INPUT_DIR/$file" "temp_outputs/${filename}_ref_sim_step.txt"; then
                    if diff "temp_outputs/${filename}_my_sim_step.txt" "temp_outputs/${filename}_ref_sim_step.txt" > /dev/null; then
                        echo "✅ $filename: Step by step OK" >> $LOG_FILE
                    else
                        echo "❌ $filename: Step by step DIFFER" >> $LOG_FILE
                        diff "temp_outputs/${filename}_my_sim_step.txt" "temp_outputs/${filename}_ref_sim_step.txt" >> $LOG_FILE
                    fi
                fi
            fi

            if run_full_program "./src/sim" "$INPUT_DIR/$file" "temp_outputs/${filename}_my_sim_full.txt"; then
                if run_full_program "./ref_sim_x86" "$INPUT_DIR/$file" "temp_outputs/${filename}_ref_sim_full.txt"; then
                    if diff "temp_outputs/${filename}_my_sim_full.txt" "temp_outputs/${filename}_ref_sim_full.txt" > /dev/null; then
                        echo "✅ $filename: Full execution OK" >> $LOG_FILE
                    else
                        echo "❌ $filename: Full execution DIFFER" >> $LOG_FILE
                        diff "temp_outputs/${filename}_my_sim_full.txt" "temp_outputs/${filename}_ref_sim_full.txt" >> $LOG_FILE
                    fi
                fi
            fi

            echo "----------------------------------------" >> $LOG_FILE
        else
            echo "⚠️  Skipping $file: File does not exist" >> $LOG_FILE
        fi
    done
    echo "" >> $LOG_FILE
done

# Podés borrar esto si querés guardar los txt para inspeccionar
rm -rf temp_outputs

echo "📝 Resultados guardados en $LOG_FILE"
