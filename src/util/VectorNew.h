#ifndef _VECTOR_NEW_H
#define _VECTOR_NEW_H

/*!
	\class Vector
	\brief A generic vector implementation based on std::vector
*/

#include <vector>

template<typename Value>
class VectorNew : public std::vector<Value> {
public:
	VectorNew(size_t chunkSize = 10):std::vector<Value>(chunkSize){}
	
	int Count() { return (int)this->size(); } 
	
	Value Last() { return this->back(); }
	
	void Add(const Value& v){ this->push_back(v); }
	
	void Erase(int position){
			this->erase(std::next(this->begin(), position));
		 }
	
private:
		std::vector<Value>	_vector;
};

#endif	// _VECTOR_NEW_H
