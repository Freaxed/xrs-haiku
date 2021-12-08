#ifndef VALUABLE_H
#define VALUABLE_H

#include <SupportKit.h>
#include <utility>

#define VAL_DATA_KEY 			"valuable:value"
#define VAL_ID       			"valuable:vID"
#define MSG_VALUABLE_CHANGED 	'valc'


typedef BString ValuableID;
class ValuableManager;

// This template is a wrapper to quickly create some 'data'

template<class TYPE>
class Valuable : public BMessage {
	public:
			Valuable(ValuableID vID, TYPE initValue):BMessage(MSG_VALUABLE_CHANGED) { 
					InitID(vID); 
					InitValue(initValue);
			}

			Valuable(ValuableID vID, TYPE initValue1, TYPE initValue2):BMessage(MSG_VALUABLE_CHANGED) {
					InitID(vID);
					InitValue(initValue1);
                    InitValue(initValue2);
			}

			static status_t FindValue(BMessage* msg, int32 index, TYPE* output);
			
	private:
		void InitID(ValuableID vID){ this->AddString(VAL_ID, vID); }
		
		void InitValue(TYPE v);
};




// TOOLS

class ValuableTools {
	public:   
    template<class TYPE>
    static bool
		SearchValues(ValuableID vID, BMessage* msg, TYPE *outData1, TYPE *outData2 = NULL, int32* countFound = NULL);

};

//experimental
template<class TYPE>
bool
ValuableTools::SearchValues(ValuableID vID, BMessage* msg, TYPE *outData1, TYPE *outData2, int32* countFound) {
    	ValuableID foundID;
		if (countFound != NULL) *countFound = 0;
        if (outData1 == NULL) return false;
        if (msg->FindString(VAL_ID, &foundID) == B_OK && foundID.Compare(vID) == 0) {
        	if (Valuable<TYPE>::FindValue(msg, 0, outData1) == B_OK) {
                if (outData2 != NULL) {
                    if (Valuable<TYPE>::FindValue(msg, 1, outData2) == B_OK) {
                    	if (countFound != NULL) *countFound = 2;
                        return true;
                    } else {
                    	*outData2 = *outData1;
                    }
                }
                if (countFound != NULL) *countFound = 1;
                return true;
        	}
        }
        return false;	
}



#endif //VALUABLE_H
