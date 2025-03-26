sim: shell.c sim.c utils/additionals.c utils/branches.c utils/load_n_store.c utils/log_shift.c utils/logical_operations.c utils/math_operations.c utils/structures.c
	gcc -g -O0 -I. $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ sim
