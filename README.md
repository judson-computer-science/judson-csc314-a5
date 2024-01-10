# Assignment 5 - Checksum validation program

The program will be written in C.

## Overview
In this assignment you will create a program that will evaluate a buffer of data that represents a product order.  The data has several fields including a checksum field that must be validated before the contents of the headers can be trusted.  After validating the checksum, you will deserialize the message into a structure, validate the payload and then print the contents of all the fields if each validation has passed.  Your program will call a provided function to get each buffer and will continue to process new buffers until none remain.


## Input
Your program will receive its input by calling the `next_order()` function provided in the `order.h` header file.

 - `next_order(unsigned char *)`
   - This function populates the provided buffer with the contents of an Order.  
   - **Returns**:  the address of the populated buffer, or NULL if there are no more Orders to populate.
   - **Parameters**:
      1. The address of a buffer to be populated with order data.


The structure of the Order follows:

		   BYTE 1      BYTE 2      BYTE 3      BYTE 4
		+-----------+-----------+-----------+-----------+
		| pppp xxxx | cccc cccc | cccc cccc | iiii iiii |
		+-----------+-----------+-----------+-----------+
		| iiii iiii | vvvv vvvv | vvvv vvvv | eeee eeee |
		+-----------+-----------+-----------+-----------+
		| rrrr rrrr   rrrr rrrr   rrrr rrrr   rrrr rrrr |
		|		      ....
		| rrrr rrrr   rrrr rrrr   rrrr rrrr   rrrr rrrr |
		+-----------+-----------+-----------+-----------+

		x = unused bit
		p = product_length (this specifies the number of bits used to describe
		    the length of the product_id field.  For example, if this field
		    contains the value 0b0110, the you shoud use the 6 least significant
		    bits of the 'product_id' field to indicate its value.  See the sample
		    output for examples.
		c = the 16 bit checksum
		i = the product_id.  Note that only 'p' bits are used as the product ID,
		    where 'p' is the number represented in the product_length field.
		v = the invoice_number
		e = Expected number of products
		r = the payload containing a comma delimited list of products

## Implementation Notes:
Your program should continue calling `next_order()` to have it's buffer populated
with the next order.  Once it has the order, it should perform the following steps:

	Step 1.) print the message buffer
	Step 2.) validate the checksum:  If invalid, print error and move on to
		 the next order.
	Step 3.) deserialize the order
	Step 4.) validate the number of items in the 'payload' against the number of
		 items specified in the 'count' field of the struct Order.  If
		 the numbers differ, print and error and move on to the next order.
		 Note that the payload contains a comma-delimited list of items so
		 you will need to tokenize the string and count tokens to determine
		 how many are actually there.
	Step 5.) If all previous validations pass, print the message using the
		 print_message() function.

In order to access the `next_order()` function, you will need to include the 'order.h' header file provided to you in the starter repository.

	Also contained within the header file are the following:

		#define HDR_SIZE  8
		#define PYLD_SIZE 112
		#define BUFF_SIZE 120

		struct Order {
			uint8_t  product_len;	// 4 MSB = # bits in product_id
			uint16_t checksum;
			uint16_t product_id;  	// 'product_len' bits of product_id
						// the remaining MSBs are unused
			uint16_t invoice_number;
			uint8_t  count;  	// count of products in the payload
			char payload[PYLD_SIZE];
		};

### Functions
Your program must provide implementations for the following functions:

 - `void print_bytes(const char *, unsigned char *, int);`
   - This should be the same function you created in assignment 3 Refer to that assignment for details (you are free to copy/paste your implementation from that assignment, or put it into a custom header file, if it is working correctly).

 - `void print_message(struct Order *);`
   - This function takes a populated struct Order address and prints the fields of the Order according to the output below.
   - **Returns**:  nothing
   - **Parameters**:
      1. the address of struct Order to print
		

 - `void unpack_message(struct Order *, unsigned char *);`
   - This function deserializes the buffer into the struct Order whose address is provided.
   - **Returns** :  nothing
   - **Parameters**:
      1. the address of struct Order to populate
      2. the address of the buffer to be deserialized


 - `int  checksum_valid(unsigned char *, int);`
   - This function computes the checksum of the buffered message and compares it with the value that was originally populated in the message.  If the values match, it returns 1.  If the values do not match, it returns a zero value (indicating FALSE);
   - **Note**:	The method for computing the checksum is the same as the method provided in the class notes:
      1. compute the one's complement sum of the header fields (don't forget to zero out the original checksum so that it's not included in your calculation)
      2. take the one's complement of the result.
   - **Returns**:  1 if the checksums match, 0 if they do not match 
   - **Parameters**:
      1. the address of the buffered message
      2. the number of bytes of the buffered message


## Output:
Your program should produce the following output:

	=====================
	Processing Message 1
	=====================
	raw buffer (120 byte buffer):
	
	0000  f0 9e ef ff ff dd cc 50   41 41 41 2c 41 41 41 2c 
	0010  41 41 41 2c 41 41 41 2c   41 41 41 2c 41 41 41 2c 
	0020  41 41 41 2c 41 41 41 2c   41 41 41 2c 41 41 41 2c 
	0030  41 41 41 2c 41 41 41 2c   41 41 41 2c 41 41 41 2c 
	0040  41 41 41 2c 41 41 41 2c   41 41 41 2c 41 41 41 2c 
	0050  41 41 41 2c 41 41 41 00   00 00 00 00 00 00 00 00 
	0060  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
	0070  00 00 00 00 00 00 00 00 
	
	Validating Checksum:
	
	 >> 00000 + 0f000 = 0f000
	 >> 0f000 + 000ff = 0f0ff
	 >> 0f0ff + 0ffdd = 1f0dc
	 >> 1f0dc + 0cc50 = 2bd2c
	
	 >> 1's complement sum [bd2e]
	 >> Computed checksum  [42d1]
	 >> Original checksum  [9eef]
	
	ERROR: computed checksum not matched!
	
	
	=====================
	Processing Message 2
	=====================
	raw buffer (120 byte buffer):
	
	0000  f0 43 0f ff ff dd cc 12   41 41 41 2c 41 41 41 2c 
	0010  41 41 41 2c 41 41 41 2c   41 41 41 2c 41 41 41 2c 
	0020  41 41 41 2c 41 41 41 2c   41 41 41 2c 41 41 41 2c 
	0030  41 41 41 2c 41 41 41 2c   41 41 41 2c 41 41 41 2c 
	0040  41 41 41 2c 41 41 41 2c   41 41 41 2c 41 41 41 2c 
	0050  41 41 41 2c 41 41 41 00   00 00 00 00 00 00 00 00 
	0060  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
	0070  00 00 00 00 00 00 00 00 
	
	Validating Checksum:
	
	 >> 00000 + 0f000 = 0f000
	 >> 0f000 + 000ff = 0f0ff
	 >> 0f0ff + 0ffdd = 1f0dc
	 >> 1f0dc + 0cc12 = 2bcee
	
	 >> 1's complement sum [bcf0]
	 >> Computed checksum  [430f]
	 >> Original checksum  [430f]
	
	ERROR: Expected 18 records but found 20
	
	
	=====================
	Processing Message 3
	=====================
	raw buffer (120 byte buffer):
	
	0000  70 9e ef ff ff 01 f1 0e   42 42 42 2c 42 42 42 2c 
	0010  42 42 42 2c 42 42 42 2c   42 42 42 2c 42 42 42 2c 
	0020  42 42 42 2c 42 42 42 2c   42 42 42 2c 42 42 42 2c 
	0030  42 42 42 2c 42 42 42 2c   42 42 42 2c 42 42 42 00 
	0040  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
	0050  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
	0060  00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00 
	0070  00 00 00 00 00 00 00 00 
	
	Validating Checksum:
	
	 >> 00000 + 07000 = 07000
	 >> 07000 + 000ff = 070ff
	 >> 070ff + 0ff01 = 17000
	 >> 17000 + 0f10e = 2610e
	
	 >> 1's complement sum [6110]
	 >> Computed checksum  [9eef]
	 >> Original checksum  [9eef]
	
	Invoice Number:      497
	Product ID Length:     7 (0007)
	Product ID:          127 (007f)
	Unit Count:           14
	Descriptions:      BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB
