#include "ImageCache.h"

#include <AppDefs.h>
#include <Bitmap.h>
#include <Debug.h>
#include <TranslationUtils.h>

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
	iterator iter = m_bitmaps.Begin();
	
	while( iter != m_bitmaps.End() ) {
		const BBitmap* bit =  iter->Value() ;
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

	const BBitmap *bitmap = NULL;
	iterator iter = m_instance->m_bitmaps.Find(name);
	

	if( iter == m_instance->m_bitmaps.End() ){
		bitmap = LoadImage(which.String(),name.String());
		if(bitmap) 	
			m_instance->m_bitmaps.Insert(name,bitmap);
	} else
		bitmap = iter->Value();

	return bitmap;
}


void
ImageCache::AddImage(BString name,BBitmap* which){

	if (m_instance == NULL)
		m_instance = new ImageCache();
		
	m_instance->m_bitmaps.Insert(name, which);		
}

void
ImageCache::DeleteImage(BString name){

	if (m_instance == NULL)
		m_instance = new ImageCache();
		
	const BBitmap* bitmap = NULL;
	iterator iter = m_instance->m_bitmaps.Find(name);	
	
	if(iter != m_instance->m_bitmaps.End()){
		bitmap = iter->Value();
		m_instance->m_bitmaps.Remove(name);
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
		printf("ImageCache: Can't load bitmap! (nor [%s] nor [resources:PNG :%s]\n",fullName,shortName);

	return bitmap;
}

// END - ImageCache.cpp
