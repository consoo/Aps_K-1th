
class CDemosaic
{
	private:
		short *bBuffer;
		short *gBuffer;
		short *rBuffer;

		int image_width, image_height;

		void interpolateLuminance(short *);
		void interpolateChrominance(short *);
	public:
		CDemosaic();
		~CDemosaic();
		void demosaic(unsigned char *, unsigned char *, int, int);
};

#define V_OFFSET 0
