#include <linux/mutex.h>
#include "lkmasg1.c"
#define MAX_MESSAGE_SIZE 1024

// Global variables
extern static char message[MAX_MESSAGE_SIZE + 1];
extern static short size_of_message;

// The mutex that locks them
extern struct mutex *mutex_ptr;

/*
 * Writes to the device
 */
static ssize_t write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	// Wait to acquire the lock
	if (!mutex_trylock(mutex_ptr)) {
		printk(KERN_ALERT "Tried to read while writing!");
	}

	int available_space_in_message = MAX_MESSAGE_SIZE - size_of_message;
	int num_chars_to_write = min(available_space_in_message, len);
	
	// Copy from the buffer only what is still available, and
	// add the null-sentinel at the end.
	strncpy(message + size_of_message, buffer, num_chars_to_write);
	message[size_of_message + num_chars_to_write] = 0;
	
	// Update size_of_message
	size_of_message = strlen(message);

	printk(KERN_INFO "%d\n", size_of_message);

	// Now that we finished, we can unlock
	mutex_unlock(mutex_ptr);

	printk(KERN_INFO "write stub");
	return num_chars_to_write;
}
