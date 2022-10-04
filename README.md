# sd-project1

## Run these commands to compile the tests
make && gcc -g -Wall tests/test_data.c -o bin/test_data obj/data.o -Iinclude && ./bin/test_data
make && gcc -g -Wall tests/test_entry.c -o bin/test_entry obj/data.o obj/entry.o -Iinclude && ./bin/test_entry
make && gcc -g -Wall tests/test_tree.c -o bin/test_tree obj/tree.o -Iinclude && ./bin/test_tree



make && gcc -g -Wall tests/test_entry.c -o bin/test_entry obj/data.o obj/entry.o -Iinclude && ./bin/test_entry


## TODO
Data ✅
Entry ✅
Tree
	Get values
Serialization
Deserialization
