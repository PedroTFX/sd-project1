# sd-project1

How to run all tests project:

 - make clean
 - make setup
 - make test_serial_run

How to run Valgrind:

 - make clean(if not done yet)
 - make setup(if not done yet)
 - make test_serial_run
 - valgrind --leak-check=full bin/test_data
 - valgrind --leak-check=full bin/test_entry
 - valgrind --leak-check=full bin/test_tree
 - valgrind --leak-check=full bin/test_serialization
