/*
 * circularbuffer.h
 *
 * Created: 12-May-25
 *  Author: PCP
 */ 

#include<stdint.h>
#include<stdbool.h>
#include<stdio.h>

/**
 * \brief struct for holding the circular buffer
 */
typedef struct circular_buffer 
{
    uint8_t *buffer;
	uint8_t size;
    volatile uint8_t write_index;
	volatile uint8_t read_index;
	
}circular_buffer_t;

 /*void circular_buffer_init(circular_buffer_t *ptr, uint8_t *buffer, uint8_t size);
 void circular_buffer_put(circular_buffer_t *ptr, uint8_t data);
 uint8_t circular_buffer_get(circular_buffer_t *ptr);*/

 /**
 * \brief Function to get the next index in a circular buffer.
 *
 * \param index the current index in the circular buffer
 * \param size the size of circular buffer
 *
 * \returns next index or 0 if we are wrapping
 */
static inline uint8_t get_next_index(uint8_t index, uint8_t size)
{
	return (index == size ? 0 : index + 1);
}

/**
 * \brief Function to get the next write offset in a circular buffer.
 *
 * \param ptr pointer to a struct of type circular_buffer
 *
 * \returns the next write offset in the circular buffer
 */
static inline uint8_t circular_buffer_get_next_write(const circular_buffer_t *ptr)
{
	return get_next_index(ptr->write_index, ptr->size);
}

/**
 * \brief Function to get the next read offset in a circular buffer.
 *
 * \param ptr pointer to a struct of type circular_buffer
 *
 * \returns the next read offset in the circular buffer
 */
static inline uint8_t circular_buffer_get_next_read(const circular_buffer_t *ptr)
{
	return get_next_index(ptr->read_index, ptr->size);
}

/**
 * \brief Function for checking if the circular buffer is full
 *
  * \param ptr pointer to a struct of type circular_buffer
 *
 * \retval true  if the buffer is full
 * \retval false if there is space available in the circular buffer
 */
static inline bool is_full_circular_buffer(const circular_buffer_t *ptr)
{
	return (ptr->read_index == circular_buffer_get_next_write(ptr));
}

/**
 * \brief Function for checking if the circular buffer is empty
 *
 * \param ptr pointer to a struct of type circular_buffer
 *
 * \retval true   if the buffer is empty
 * \retval false  if there is still data in the buffer
 */
static inline bool is_empty_circular_buffer(const circular_buffer_t *ptr)
{
	return (ptr->read_index== ptr->write_index);
}

/**
 * \brief Function for initializing a circular buffer
 *
 * \param buffer pointer to the buffer to use as a circular buffer
 * \param size   the size of the circular buffer
 *
 * \retval struct circular_buffer a struct containing the circular buffer
 */
static inline void circular_buffer_init(circular_buffer_t *ptr, uint8_t *buffer, uint8_t size)
{
	//circular_buffer_t cbuffer;
	ptr->write_index = 0;
	ptr->read_index = 0;
	ptr->size = size;
	ptr->buffer = buffer;
	//return cbuffer;
}

/**
 * \brief Function for getting one byte from the circular buffer
 *
 * \param ptr pointer to a struct of type circular_buffer
 *
 * \returns data byte in buffer
 */
static inline uint8_t circular_buffer_get(circular_buffer_t *ptr, uint8_t *valPtr)
{
	//Assert(!is_empty_circular_buffer(ptr));
	if(!is_empty_circular_buffer(ptr))
	{
		*valPtr= ptr->buffer[ptr->read_index];
		ptr->read_index= circular_buffer_get_next_read(ptr);
		return 1;
	}
	else
	{
        //printf("Buffer Empty\n");
		return 0;
	}
}

/**
 * \brief Function for putting a data byte in the circular buffer
 *
 * Call this function to put a byte of data in the circular buffer.
 * Make sure buffer is not full (using \ref circular_buffer_is_full)
 * before calling this function.
 *
 * \param ptr pointer to a struct of type circular_buffer
 * \param data the byte to put to the buffer
 *
 */
static inline uint8_t circular_buffer_put(circular_buffer_t *ptr, uint8_t data)
{
	//Assert(!is_full_circular_buffer(ptr));
	if(!is_full_circular_buffer(ptr))
	{
		ptr->buffer[ptr->write_index] = data;
		ptr->write_index = circular_buffer_get_next_write(ptr);
        return 1;
	}
    else
    {
        //printf("Buffer Full\n");
        return 0;
    }

	
}