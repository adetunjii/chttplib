STD=-pedantic

test-request:
	$(CC) $(STD) -DTEST_REQUEST -o request request.c reader.c