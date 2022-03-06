#include "ImageCache.h"

#include <AppDefs.h>
#include <Bitmap.h>
#include <Debug.h>
#include <TranslationUtils.h>
#include "Log.h"

using namespace std;

// ---------------------------------------------------------------------------
// Class Data Initialization

ImageCache *
ImageCache::m_instance = NULL;

// ---------------------------------------------------------------------------
// Constructor/Destructor

ImageCache::ImageCache() {

}

ImageCache::~ImageCache()
{
	iterator iter = m_bitmaps.begin();
	
	while( iter != m_bitmaps.end() ) {
		const BBitmap* bit =  iter->second ;
		delete bit;
		iter++;
	}
	
	m_bitmaps.MakeEmpty();
}

// ---------------------------------------------------------------------------
// Operations

const BBitmap *
ImageCache::GetImage(
	BString which, BString name)
{
	if (m_instance == NULL)
		m_instance = new ImageCache();

	/* cerca.. se non c'e' lo carica altrimenti usa quello */

	bool found = false;
	const BBitmap *bitmap = m_instance->m_bitmaps.ValueFor(name, &found);
	

	if( !found ){
		bitmap = LoadImage(which.String(),name.String());
		if(bitmap) 	
			m_instance->m_bitmaps.AddItem(name, bitmap);
	}
	
	return bitmap;
}


void
ImageCache::AddImage(BString name, const BBitmap* which){

	if (m_instance == NULL)
		m_instance = new ImageCache();
		
	m_instance->m_bitmaps.AddItem(name, which);		
}

void
ImageCache::DeleteImage(BString name){

	if (m_instance == NULL)
		m_instance = new ImageCache();
	
	bool found = false;
	const BBitmap* bitmap = m_instance->m_bitmaps.ValueFor(name, &found);
	
	if(found){
		m_instance->m_bitmaps.RemoveItemFor(name);
		if (bitmap)
			delete bitmap;	
	}
}

void
ImageCache::Release()
{
	if (m_instance != NULL){
		delete m_instance;
		m_instance = NULL;
	}
}

const BBitmap *
ImageCache::LoadImage(
	const char *fullName, const char* shortName) {
	const BBitmap *bitmap = BTranslationUtils::GetBitmap(fullName); //load from disk
	
	if(!bitmap) //load from resources
		bitmap = BTranslationUtils::GetBitmap('PNG ', shortName);
	
	if(!bitmap)
		LogError("ImageCache: Can't load bitmap! (nor [%s] nor [resources:PNG :%s]",fullName,shortName);

	return bitmap;
}

// END - ImageCache.cpp
