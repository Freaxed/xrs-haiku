#ifndef ImageCache_H
#define ImageCache_H

 /**	
 *		ImageCache.	
 *		@author	Andrea Anzani.   
 */
 
class BBitmap;

#include <SupportDefs.h>
#include <String.h>
#include "KeyMap.h"
//#include "VectorMap.h"

class ImageCache
{

protected:						// Constructor/Destructor

								ImageCache();

								~ImageCache();

public:							// Operations

	/** Returns the image corresponding to the which constant */
	static  const BBitmap *		GetImage( BString which ,BString name);


	static	 void AddImage(BString name, const BBitmap* which);
	static	 void DeleteImage(BString name);

	/** Frees the singleton instance of the cache;
		Call this when app quits
	 */
	static void					Release();

private:					

	static const BBitmap *	LoadImage(	const char *resourceName,const char *);
								// Class Data

	static ImageCache *		m_instance;

private:						// Instance Data


	KeyMap<BString, const BBitmap*>	m_bitmaps;
	typedef KeyMap<BString, const BBitmap*>::iterator	iterator;

};

#endif /* __C_ImageCache_H__ */
