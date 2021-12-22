/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Valuable.h"

template<class TYPE>
status_t 
Valuable<TYPE>::FindValue(BMessage* msg, int32 index, TYPE* output) { return TypeStillNotSupported(output); }

template<class TYPE>
void 
Valuable<TYPE>::InitValue(TYPE v) {  TypeStillNotSupported(v); } //add a line at the end



// Float
template<>
void Valuable<float>::InitValue(float v) { this->AddFloat(VAL_DATA_KEY, v); }

template<>
status_t Valuable<float>::FindValue(BMessage* msg, int32 index, float* output) { return msg->FindFloat(VAL_DATA_KEY, index, output);}

//Int32
template<>
void Valuable<int32>::InitValue(int32 v) {	this->AddInt32(VAL_DATA_KEY, v); }

template<>
status_t Valuable<int32>::FindValue(BMessage* msg, int32 index, int32* output) { return msg->FindInt32(VAL_DATA_KEY, index, output);}
