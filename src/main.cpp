#include <iostream>
#include "ap_int.h"
#include "utils/x_hls_utils.h"
#include<stdint.h>
#include "hls_stream.h"
#include "ap_axi_sdata.h"

typedef struct
{
	uint16_t dummyX;
	uint16_t dummyY;
	uint64_t dummyTs;
	uint8_t dummyPol;
	uint8_t dummyCorner;
} status_t;

void eventStreamSwitch(ap_uint<32> config,
		hls::stream< ap_uint<16> > &xStreamIn0, hls::stream< ap_uint<16> > &yStreamIn0,
		hls::stream< ap_uint<64> > &tsStreamIn0, hls::stream< ap_uint<8> > &polStreamIn0,
		hls::stream< ap_uint<8> > &cornerStreamIn0,
		hls::stream< ap_uint<16> > &xStreamIn1, hls::stream< ap_uint<16> > &yStreamIn1,
		hls::stream< ap_uint<64> > &tsStreamIn1, hls::stream< ap_uint<8> > &polStreamIn1,
		hls::stream< ap_uint<8> > &cornerStreamIn1,
		hls::stream< ap_uint<16> > &xStreamOut, hls::stream< ap_uint<16> > &yStreamOut,
		hls::stream< ap_uint<64> > &tsStreamOut, hls::stream< ap_uint<1> > &polStreamOut,
		hls::stream< ap_uint<1> > &cornerStreamOut,
		ap_uint<16> *outputX, ap_uint<16> *outputY,
		ap_uint<64> *outputTs, ap_uint<1> *outputPol,
		ap_uint<1> *outputCor)
{
#pragma HLS PIPELINE
#pragma HLS INTERFACE s_axilite port=config bundle=config
//#pragma HLS INTERFACE s_axilite port=status bundle=config

#pragma HLS INTERFACE axis register both port=cornerStreamOut
#pragma HLS INTERFACE axis register both port=tsStreamOut
#pragma HLS INTERFACE axis register both port=polStreamOut
#pragma HLS INTERFACE axis register both port=yStreamOut
#pragma HLS INTERFACE axis register both port=xStreamOut

#pragma HLS INTERFACE axis register both port=cornerStreamIn1
#pragma HLS INTERFACE axis register both port=polStreamIn1
#pragma HLS INTERFACE axis register both port=tsStreamIn1
#pragma HLS INTERFACE axis register both port=yStreamIn1
#pragma HLS INTERFACE axis register both port=xStreamIn1

#pragma HLS INTERFACE axis register both port=cornerStreamIn0
#pragma HLS INTERFACE axis register both port=polStreamIn0
#pragma HLS INTERFACE axis register both port=tsStreamIn0
#pragma HLS INTERFACE axis register both port=yStreamIn0
#pragma HLS INTERFACE axis register both port=xStreamIn0

	ap_uint<16> x, y;
	ap_uint<16> x0, x1, y0, y1;
	ap_uint<1> pol;
	ap_uint<8> pol0, pol1;
	ap_uint<64> ts;
	ap_uint<64> ts0, ts1;
	ap_uint<1> corner;
	ap_uint<8> cornerIn0, cornerIn1;

	static ap_uint<16> dummyX, dummyY;
	static ap_uint<8> dummyPol;
	static ap_uint<64> dummyTs;
	static ap_uint<8> dummyCorner;

	ap_uint<1> select = config[0]; // config bit 0. 0 : real-time mode (default). 1: file playing mode

	if(select == 0)
	{
		xStreamIn0 >> x0;
		yStreamIn0 >> y0;
		polStreamIn0 >> pol0;
		tsStreamIn0 >> ts0;
		cornerStreamIn0 >> cornerIn0;

		// Read dummy data from the other channel using non-block read
		xStreamIn1.read_nb(dummyX);
		yStreamIn1.read_nb(dummyY);
		polStreamIn1.read_nb(dummyPol);
		tsStreamIn1.read_nb(dummyTs);
		cornerStreamIn1.read_nb(dummyCorner);

		// config bit 1. 1: forward mode (flag all events to corners). 0: corner filter mode (only flag real corner events)
		if(config[1] == 1)
		{
			cornerIn0 = 1;
		}

		x = x0;
		y = y0;
		pol = pol0;
		ts = ts0;
		corner = cornerIn0;
	}
	else
	{
		xStreamIn1 >> x1;
		yStreamIn1 >> y1;
		polStreamIn1 >> pol1;
		tsStreamIn1 >> ts1;
		cornerStreamIn1 >> cornerIn1;

		// Read dummy data from the other channel using non-block read
		xStreamIn0.read_nb(dummyX);
		yStreamIn0.read_nb(dummyY);
		polStreamIn0.read_nb(dummyPol);
		tsStreamIn0.read_nb(dummyTs);
		cornerStreamIn0.read_nb(dummyCorner);

		// config bit 1. 1: forward mode (flag all events to corners). 0: corner filter mode (only flag real corner events)
		if(config[1] == 1)
		{
			cornerIn1 = 1;
		}

		x = x1;
		y = y1;
		pol = pol1;
		ts = ts1;
		corner = 1;
		corner = cornerIn1;
	}

	xStreamOut << x;
	yStreamOut << y;
	tsStreamOut << ts;
	polStreamOut << pol;
	cornerStreamOut << corner;

	 *outputX = dummyX;
	 *outputY = dummyY;
	 *outputTs = dummyTs;
	 *outputPol = dummyPol;
	 *outputCor = dummyCorner;
//	(*status).dummyX = dummyX;
//	(*status).dummyY = dummyY;
//	(*status).dummyTs = dummyTs;
//	(*status).dummyPol = dummyPol;
//	(*status).dummyCorner = dummyCorner;
}
