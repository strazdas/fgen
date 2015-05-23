compile:
	g++ gkls.c rnd_gen.c gkls_eval.cpp -I. -ogkls_eval
	g++ gkls.c rnd_gen.c gkls_get_glob_min.cpp -I. -ogkls_get_glob_min
	# dimension, global_dist, global_radius, num_minima, function_id, point coordinates 
	./gkls_eval 2 0.9 0.2 10 1 0 0
	# dimension, global_dist, global_radius, num_minima, function_id
	./gkls_get_glob_min 2 0.9 0.2 10 1
