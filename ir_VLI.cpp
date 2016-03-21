/*
 * ir_VLI.cpp
 *
 *  2016
 *  Vincent Limorté
 *  Content under GPL v3 : http://www.gnu.org/licenses/gpl-3.0.fr.html
 */

/*
 * Protocol :
 * Header/8bit-message/parity bit/footer
 * header : --_--_
 * 1 : -_
 * 0 : -___
 * footer : -_-_
 */

#define VLI_QUANTUM 250
#define VLI_HDR_MARK (2*VLI_QUANTUM)
#define VLI_ZERO_SPACE (3*VLI_QUANTUM)

#if SEND_VLI
	void IRsend:sendVLI(uint8_t data, int freq_khz /*=38*/){
		// Set IR carrier frequency
			enableIROut(freq_khz);

			// Header
			mark (VLI_HDR_MARK);
			space(VLI_QUANTUM);
			mark (VLI_HDR_MARK);
			space(VLI_QUANTUM);
			bool parity=false;
			// Data
			for (unsigned long  mask = 1UL << (7);  mask;  mask >>= 1) {
				if (data & mask) {
					mark (VLI_QUANTUM);
					space(VLI_QUANTUM);
					parity = !parity;
				} else {
					mark (VLI_QUANTUM);
					space(VLI_ZERO_SPACE);
				}
			}

			if(parity){
				mark (VLI_QUANTUM);
				space(VLI_QUANTUM);
			} else {
				mark (VLI_QUANTUM);
				space(VLI_ZERO_SPACE);
			}
			// Footer
			mark (VLI_QUANTUM);
			space(VLI_QUANTUM);
			mark (VLI_QUANTUM);
			space(VLI_QUANTUM);

	}

#endif


#if DECODE_VLI
	bool  decodeVLI (decode_results *results){
		// go to first mark
		int offset=1;
		uint8_t data=0;

		// Check we have enough data
		if (irparams.rawlen < 4 + 16 + 2 + 4 )  return false ;

		// process header
		if(!MATCH_MARK(results->rawbuff[offset++],VLI_HDR_MARK)) return false;
		if(!MATCH_SPACE(results->rawbuff[offset++],VLI_QUANTUM)) return false;
		if(!MATCH_MARK(results->rawbuff[offset++],VLI_HDR_MARK)) return false;
		if(!MATCH_SPACE(results->rawbuff[offset++],VLI_QUANTUM)) return false;

		// data
		for(int i=7; i>=0; i--){
			if(!MATCH_MARK(results->rawbuff[offset++],VLI_HDR_MARK)) return false;
			if(MATCH_SPACE(results->rawbuff[offset++],VLI_QUANTUM)){
				// 1
				data |= 1<<(i)
			} else if(MATCH_SPACE(results->rawbuff[offset++],VLI_ZERO_SPACE)){
				//0
			} else {
				return false;
			}l
		}

		// parity check


		//footer

	}
#endif

