sim: shell.c sim.c src/additionals.c src/branches.c src/load_n_store.c src/log_shift.c src/logical_operations.c src/math_operations.c src/structures.c
	gcc -g -O0 $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ sim
