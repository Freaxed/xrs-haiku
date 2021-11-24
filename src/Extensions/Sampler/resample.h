#ifndef _RESAMPLE_H
#define _RESAMPLE_H




/**
 * Refiller interface
 *
 * this is used by the resampler class
 */
class Refiller {
public:
	/**
	 * callback method to supply data to the Resampler
	 *
	 * here you supply data in the encoding specified in the Resampler class
	 * (bits, channels, endianness) to the Resampler
	 *
	 * @param buffer a pointer to a buffer where the data can be filled to
	 * @param len the size of the buffer in bytes
	 *
	 * @returns the number of bytes really available
	 */
	virtual unsigned long read(unsigned char *buffer, unsigned long len,const void* cookie) = 0;
	virtual ~Refiller();
};

class ResamplerPrivate;

/**
 * The Resampler class resamples data from an input stream, which is read
 * through the Refiller interface, and renders it into the float format which
 * aRts usually uses, converting the sampling rate and format as needed.
 */
class Resampler {
protected:
	static const unsigned int bufferSize = 256;		//  64 samples in buffer
	static const unsigned int bufferWrap = 64;		// + "wrap around"
	/*
	 * BC: bufferWrap just needs channels * (filtersize - 1) * bytes per sample
	 * bytes wrap around - this used to be 4: 2 channels, linear interpolation,
	 * 16bit. However, for supporting float resampling and/or better filters,
	 * we'll make this 64 to be safe.
	 */

	int bufferSamples;
	int sampleSize;
	int dropBytes;

	Refiller *refiller;
	double pos, step;
	int channels,bits;
	
	const void*	cookie;

	unsigned char buffer[bufferSize+bufferWrap];
	float fbuffer[bufferSize+bufferWrap];
	long block, haveBlock;

	class ResamplerPrivate *d;

	void updateSampleSize();
	void ensureRefill();
public:
	/**
	 * constructor
	 *
	 */
	Resampler(Refiller *refiller,const void* cookie);
	~Resampler();

	/**
	 * sets the resampling step in the input stream
	 *
	 * for instance, settings step to 2.0, the file would be played twice as
	 * fast - you can calculate the resampling step from the sampling rate
	 * the stream you resample was recorded at, and the sampling rate you want
	 * it to be resampled to, using
	 *
	 *   step = originalSamplingRate / desiredSamplingRate
	 *
	 * for instance, if a stream recorded at 22050 Hz is to be played at 48000
	 * Hz, step would be
	 *
	 *   step = 22050 / 48000 = 0.46
	 */
	void setStep(double step);

	/**
	 * sets the number of channels in the input stream
	 *
	 * the channels are expected to be interleaved, thus if you have two
	 * channels, you will have one sample for the left stream, one for the
	 * right, one for the left, ... and so on
	 *
	 * currently only either mono (channels = 1) or stereo (channels = 2)
	 * streams are supported
	 */
	void setChannels(int channels);

	/**
	 * sets the number of bits in the input stream
	 *
	 * currently, only 16 bit and 8 bit streams are supported
	 */
	void setBits(int bits);

	enum Endianness { bigEndian, littleEndian };

	/**
	 * sets the endianess of the input stream
	 *
	 * supported are bigEndian and littleEndian
	 */
	void setEndianness(Endianness endianness);

	/**
	 * renders a buffer of float samples from the input stream
	 *
	 * the data is obtained by querying the refiller for data from the
	 * input stream and filling them into the float array pointed to by
	 * left and right - if reading data fails, the underrun flag is set
	 */
	unsigned long run(float *left, float *right, unsigned long samples);

	/**
	 * query the underrun flag
	 *
	 * underruns occur if run is called, but no data is available from the
	 * Refiller object
	 */
	bool underrun();
};


#endif /* _RESAMPLE_H */
