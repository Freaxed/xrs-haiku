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
	VectorNew():std::vector<Value>(){}
	
	int Count() { return (int)this->size(); } 
	
	Value Last() { return this->back(); }
	
	void Add(const Value& v){ this->push_back(v); }
	
	void Erase(int position){
			this->erase(std::next(this->begin(), position));
		 }
	void	MakeEmpty() { this->clear(); }
	
	int 	IndexOf(Value v) 
	{
		for(int j=0;j<Count();j++)
		{
			if (v == (*this)[j])
				return j;
		} 
		return -1;
	}

};

#endif	// _VECTOR_NEW_H
