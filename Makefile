STD=-pedantic

test-request:
	cd src && $(CC) $(STD) -DTEST_REQUEST -o request request.c reader.c