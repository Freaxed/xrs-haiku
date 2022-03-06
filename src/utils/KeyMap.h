#ifndef  KeyMap_H_
#define KeyMap_H_

#include <map>
#include <SupportDefs.h>

using namespace std;


template<class KEY,class TYPE>
class KeyMap : public map<KEY, TYPE> 
{
 public:
			uint32 		CountItems();
		    void  		AddItem(KEY k, const TYPE t);
		    const TYPE		ValueFor(KEY,bool* found=NULL);
		    void		RemoveItemAt(int32 position);
		    void		RemoveItemFor(KEY);
		    const TYPE		ValueAt(int32 position);
		    KEY			KeyAt(int32 position);
		    void		MakeEmpty() { this->clear(); }
		   // int32		IndexOf(KEY);
private:
			typedef typename map<KEY, TYPE>::iterator	mapIter;
};

template<class KEY,class TYPE>
uint32 KeyMap<KEY,TYPE>::CountItems()
{
	return this->size();
}

template<class KEY,class TYPE>
void KeyMap<KEY,TYPE>::AddItem(KEY k, const TYPE t)
{
	(*this)[k] = t;
}

template<class KEY,class TYPE>
const TYPE KeyMap<KEY,TYPE>::ValueFor(KEY k,bool* found){
	
	mapIter i = this->find(k);
	
	if(found) {
		if(i == this->end())	*found=false;	
		else  *found=true;
	}
	if(i == this->end())
		return NULL;
	else
		return i->second; // fMap[k];
}


template<class KEY,class TYPE>
void KeyMap<KEY,TYPE>::RemoveItemAt(int32 position){
	iterator i= this->begin();
	std::advance(i, position);
	this->erase(i->first);
}

template<class KEY,class TYPE>
void KeyMap<KEY,TYPE>::RemoveItemFor(KEY k){
	this->erase(k);
}

template<class KEY,class TYPE>
const TYPE KeyMap<KEY,TYPE>::ValueAt(int32 position){
	iterator i= this->begin();
	std::advance(i, position); 	
	if(i == this->end())
		return NULL;
	else
		return i->second;
}

template<class KEY,class TYPE>
KEY KeyMap<KEY,TYPE>::KeyAt(int32 position){
	iterator i= this->begin();
	std::advance(i, position); 	
	if(i == this->end())
		return NULL;
	else
		return i->first;
}


#endif
//--
