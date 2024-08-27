STD=-pedantic

test-request:
	cd src && $(CC) $(STD) -DTEST_REQUEST -o request reader.c url.c request.c palloc.c ehandle.c

format:
	clang-format -style=file -i **/*.c **/*.h