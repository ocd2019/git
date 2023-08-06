#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#define	SIGN_BIT	(0x80)		/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	NSEGS		(8)			/* Number of A-law segments. */
#define	SEG_SHIFT	(4)			/* Left shift for segment number. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */
#define	BIAS		(0x84)		/* Bias for linear code. */

#define VOIP_G711A_LENGTH 		320
#define VOIP_PCM_LENGTH 		640

unsigned short voip_decode(unsigned char a_val)
{
	short     t;  
	int     seg;  
	
	a_val ^= 0x55;
	
	t = (a_val & QUANT_MASK) << 4;  
	seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;  
	switch (seg) {  
	case 0:  
		t += 8;  
		break;  
	case 1:  
		t += 0x108;  
		break;  
	default:  
		t += 0x108;  
		t <<= seg - 1;  
	}  
	return ((a_val & SIGN_BIT) ? t : -t);  
}


char *voip_g711a_to_pcm(char *dst_buf,unsigned char *src_buf,int src_len)
{
	int i = 0;

	unsigned char *src = src_buf;
	unsigned short len = (unsigned short)src_len;
	unsigned short *dst = (unsigned short*)dst_buf;

	for (i = 0; i < len; i++)
	{
		dst[i] = voip_decode(src[i]);
	}

	return (char *)dst;
}

static int alaw2linear( unsigned char a_val )
{
	int	t;
	int	seg;

	a_val ^= 0x55;

	t = (a_val & QUANT_MASK) << 4;
	seg = ( (unsigned)a_val & SEG_MASK ) >> SEG_SHIFT;
	switch (seg) 
	{
		case 0:
			t += 8;
			break;
		case 1:
			t += 0x108;
			break;
		default:
			t += 0x108;
			t <<= seg - 1;
	}
	return ((a_val & SIGN_BIT) ? t : -t);
}



static int g711a_decode(short amp[], const unsigned char g711a_data[], int g711a_bytes)
{
	int i;
	int samples;
	unsigned char code;
	int sl;

	for (samples = i = 0; ; )
	{
		if (i >= g711a_bytes) break;

		code = g711a_data[i++];

		sl = alaw2linear( code );

		amp[samples++] = (short) sl;
	}

	return samples * 2;
}


char *G711a2PCM(char *InAudioData, char *OutAudioData, int DataLen)
{
	if((NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen) )
	{
		printf("Error, empty data or transmit failed, exit !\n");	
		return -1;
	}
    
	int Retade = 0;

	Retade = g711a_decode((short*)OutAudioData,(unsigned char *)InAudioData,DataLen);

	//return Retade;

	return (char *)OutAudioData;
}


int main(void)
{
    int nOutLen = 0;
	char outbuf[VOIP_PCM_LENGTH];

	voip_g711a_to_pcm(outbuf,message_queue.head->data,VOIP_G711A_LENGTH);

	voipAlaw2Linear(message_queue.head->data, VOIP_G711A_LENGTH, outbuf, &nOutLen);

	return 0;
}

