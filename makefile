
# mergetars.c mergetars.h run_cmd.c myfunction.c expand_tars.c compare_files.c copy_files.c create_output_tar.c cleanup.c sysbin.h

mergetars: mergetars.c mergetars.h run_cmd.c expand_tars.c compare_files.c copy_files.c create_output_tar.c cleanup.c sysbin.h
	gcc -std=c99 -D_BSD_SOURCE -o mergetars \
		mergetars.c cleanup.c run_cmd.c expand_tars.c compare_files.c copy_files.c create_output_tar.c sysbin.h mergetars.h
