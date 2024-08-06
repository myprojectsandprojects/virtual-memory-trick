/*
	Windows:

	CreateFileMapping()
	VirtualAlloc2()
	MapViewOfFile3()
*/

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// #define _GNU_SOURCE
#include <sys/mman.h>
// Why is it not in "sys/mman.h"?
// Copy-paste from docs:
int memfd_create(const char *name, unsigned int flags);

int main()
{
	printf("_SC_PAGESIZE: %ld\n", sysconf(_SC_PAGESIZE));

	int MyFile = memfd_create("my-file", 0);
	printf("memfd_create() returns: %d\n", MyFile);

	// lseek(MyFile, 0, SEEK_SET);

	// char Buffer[64+1];
	// int BytesRead = read(666, Buffer, 64);
	// if(BytesRead == -1) {
	// 	assert(false);
	// }
	// printf("read() returns: %d\n", BytesRead);
	// Buffer[BytesRead] = 0;

	// printf("Is this 'abc'?: %s\n", Buffer);

	// printf("before: %d\n", lseek(MyFile, 0, SEEK_END));
	ftruncate(MyFile, 4096);
	// printf("after: %d\n", lseek(MyFile, 0, SEEK_END));

	char FourtyTwo = 42;
	int BytesWritten = write(MyFile, &FourtyTwo, 1);

	char *BigChunk = (char *)mmap(0, 2 * 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(BigChunk == MAP_FAILED){
		printf("error: %s\n", strerror(errno));
		assert(0);
	}
	printf("BigChunk: %p\n", BigChunk);

	munmap(BigChunk, 4096);
	//@ Could fail?
	char *Ptr1 = (char *)mmap(BigChunk, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED_NOREPLACE, MyFile, 0);
	if(Ptr1 == MAP_FAILED){
		printf("error: %s\n", strerror(errno));
		assert(0);
	}
	printf("Ptr1: %p\n", Ptr1);

	munmap(BigChunk + 4096, 4096);
	//@ Could fail?
	char *Ptr2 = (char *)mmap(BigChunk + 4096, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED_NOREPLACE, MyFile, 0);
	if(Ptr2 == MAP_FAILED){
		assert(0);
	}
	printf("Ptr2: %p\n", Ptr2);

	// char Buffer[16+1];
	// int BytesRead = read(MyFile, Buffer, 16);
	// if(BytesRead == -1) {
	// 	assert(0);
	// }
	// printf("read() returns: %d\n", BytesRead);
	// Buffer[BytesRead] = 0;

	// printf("Is this 'abc'?: %s\n", Buffer);

	printf("Ptr1[0]: %d\n", Ptr1[0]);
	printf("Ptr2[0]: %d\n", Ptr2[0]);

	return 0;
}