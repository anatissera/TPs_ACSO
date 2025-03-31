sim: shell.c sim.c decoder.c utils/additionals.c utils/branches.c utils/load_n_store.c utils/log_shift.c utils/logical_operations.c utils/math_operations.c utils/structures.c
	gcc -g -O0 -I. $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ sim

fix-dos:
	dos2unix ../compare_sims.sh

run: fix-dos
# Si obtienen un error relacionado con `$'\r': command not found,
# convierten el archivo a formato Unix antes de ejecutarlo:
	dos2unix ../compare_sims.sh
	chmod +x ../compare_sims.sh