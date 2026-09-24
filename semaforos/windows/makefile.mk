CC  = cl
CFLAGS = /EHsc
TARGET = example_semaphore.exe

$(TARGET): example_semaphore.obj
	$(CC) $(CFLAGS) /Fe:$(TARGET) example_semaphore.obj

example_semaphore.obj: example_semaphore.c
	$(CC) $(CFLAGS) /c example_semaphore.c

clean:
	del *.obj $(TARGET)

