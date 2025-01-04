/*
	Receive the number of items about to be stored in a vector,
	and decide what the capacity should be,
	based on our current understanding of the vector *ADT*.

	Remember that we use two basic rules:
		* Capacity doubles when out of room
		* Capacity halves when utilized by less than a third
		* If the capacity doesn't need to be changed, just return the old value
*/
size_t computeAppropriateCapacity(size_t size, size_t current_capacity) {
	
	if (size <= current_capacity / 3) {
		return current_capacity / 2;
	}
	
	else if (size >= current_capacity) {
		return current_capacity * 2;
	}
	
	return current_capacity;	
}

/*
	Create a new array of the specified size and capacity,
		preserving all values present in the old array.

	This covers the basic idea behind situations in a vector where
		capacity changes and a new array must be created
		while preserving all values.

	Make sure you ```delete``` the old array when you are finished with it.

	Also make sure you return a pointer to the new array (you may
		assume the caller will use it somehow).
*/
template <class T>
T* recreateInternalArray(T* old_array, size_t old_size, size_t newCapacity) {
	for (size_t i = 0; i < old_size; i++) {
		recreateInternalArray[i] = old_array[i];
	}
	delete[] old_array;
	old_array = recreateInternalArray;
	current_capacity = newCapacity
}
