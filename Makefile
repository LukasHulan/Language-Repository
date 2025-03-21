main.exe: main.c parser.c test_harness.c to_string.c parse_tree.c tokenizer.c
	gcc main.c parser.c test_harness.c to_string.c parse_tree.c tokenizer.c -o main.exe