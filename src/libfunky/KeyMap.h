#ifndef  KeyMap_H_
#define KeyMap_H_

#include <map>
#include <SupportDefs.h>

using namespace std;

template<class KEY,class TYPE>
class KeyMap {

 public:
			uint32 		CountItems();
		    void  		AddItem(KEY k,TYPE t);//{fMap[k]=t;};
		    TYPE		ValueFor(KEY,bool* found=NULL);
		    void		RemoveItemsAt(int32 position);
		    void		RemoveItemFor(KEY);
		    TYPE		ValueAt(int32 position);
		    KEY			KeyAt(int32 position);
		   // int32		IndexOf(KEY);
private:
			std::map<KEY,TYPE>	fMap;
			//typedef std::map<KEY, TYPE>::iterator mIter; 
};

template<class KEY,class TYPE>
uint32 KeyMap<KEY,TYPE>::CountItems(){
	return fMap.size();
}

template<class KEY,class TYPE>
void KeyMap<KEY,TYPE>::AddItem(KEY k,TYPE t){
	fMap[k]=t;
}

template<class KEY,class TYPE>
TYPE KeyMap<KEY,TYPE>::ValueFor(KEY k,bool* found){
	
	map<KEY, TYPE>::iterator i = fMap.find(k);
	
	if(found) {
		if(i == fMap.end())	*found=false;	
		else  *found=true;
	}
	if(i == fMap.end())
		return NULL;
	else
		return i->second; // fMap[k];
}


template<class KEY,class TYPE>
void KeyMap<KEY,TYPE>::RemoveItemsAt(int32 position){
	mIter i= fMap.begin();
	std::advance(i, position);
	fMap.erase(i->first);
}

template<class KEY,class TYPE>
void KeyMap<KEY,TYPE>::RemoveItemFor(KEY k){
	fMap.erase(k);
}

template<class KEY,class TYPE>
TYPE KeyMap<KEY,TYPE>::ValueAt(int32 position){
	mIter i= fMap.begin();
	std::advance(i, position); 	
	if(i == fMap.end())
		return NULL;
	else
		return i->second;
}

template<class KEY,class TYPE>
KEY KeyMap<KEY,TYPE>::KeyAt(int32 position){
	mIter i= fMap.begin();
	std::advance(i, position); 	
	if(i == fMap.end())
		return NULL;
	else
		return i->first;
}


#endif
//--
