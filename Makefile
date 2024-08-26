STD=-pedantic

test-request:
	cd src && $(CC) $(STD) -DTEST_REQUEST -o request reader.c url.c request.c

format:
	clang-format -style=file -i **/*.c **/*.h