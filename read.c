#include <linux/mutex.h>
#include "lkmasg1.c"
#define MAX_MESSAGE_SIZE 1024

// Global variables
extern static char message[MAX_MESSAGE_SIZE + 1];
extern static short size_of_message;

// The mutex that locks them
extern struct mutex *mutex_ptr;

/*
 * Reads from device, displays in userspace, and deletes the read data
 */
static ssize_t read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	// Wait to acquire the lock
	if (!mutex_trylock(mutex_ptr)) {
		printk(KERN_ALERT "Tried to read while writing!");
	}
	
	int num_chars_to_copy = min(len, size_of_message);
	char temp[MAX_MESSAGE_SIZE];

	// Copy what we can from the stored message to the buffer
	strncpy(buffer, message, num_chars_to_copy);
	buffer[num_chars_to_copy] = 0;

	// Copy the message into temp, starting
	// from where the last copy ended
	strcpy(temp, message + num_chars_to_copy);

	// Copy temp back into message, and
	// add the null-sentinel right after
	strcpy(message, temp);
	message[size_of_message - num_chars_to_copy] = 0;

	size_of_message = strlen(message);
	
	// Now that we finished, we can unlock
	mutex_unlock(mutex_ptr);

  	printk(KERN_INFO "read stub");
	return num_chars_to_copy;
}
