#include <iostream>
#include "ap_int.h"
#include "utils/x_hls_utils.h"
#include<stdint.h>
#include "hls_stream.h"
#include "ap_axi_sdata.h"

void eventStreamSwitch(ap_uint<1> select,
		hls::stream< ap_uint<16> > &xStreamIn1, hls::stream< ap_uint<16> > &yStreamIn1,
		hls::stream< ap_uint<64> > &tsStreamIn1, hls::stream< ap_uint<1> > &polStreamIn1,
		hls::stream< ap_uint<16> > &xStreamIn2, hls::stream< ap_uint<16> > &yStreamIn2,
		hls::stream< ap_uint<64> > &tsStreamIn2, hls::stream< ap_uint<1> > &polStreamIn2,
		hls::stream< ap_uint<16> > &xStreamOut, hls::stream< ap_uint<16> > &yStreamOut,
		hls::stream< ap_uint<64> > &tsStreamOut, hls::stream< ap_uint<1> > &polStreamOut)
{
#pragma HLS PIPELINE
#pragma HLS INTERFACE axis register both port=tsStreamOut
#pragma HLS INTERFACE axis register both port=polStreamOut
#pragma HLS INTERFACE axis register both port=yStreamOut
#pragma HLS INTERFACE axis register both port=xStreamOut

#pragma HLS INTERFACE axis register both port=polStreamIn
#pragma HLS INTERFACE axis register both port=tsStreamIn
#pragma HLS INTERFACE axis register both port=yStreamIn
#pragma HLS INTERFACE axis register both port=xStreamIn

#pragma HLS INTERFACE axis register both port=polStreamIn
#pragma HLS INTERFACE axis register both port=tsStreamIn
#pragma HLS INTERFACE axis register both port=yStreamIn
#pragma HLS INTERFACE axis register both port=xStreamIn

	ap_uint<16> x, y;
	ap_uint<1> pol;
	ap_uint<64> ts;

	if(select == 0)
	{
		xStreamIn1 >> x;
		yStreamIn1 >> y;
		polStreamIn1 >> pol;
		tsStreamIn1 >> ts;
	}
	else
	{
		xStreamIn2 >> x;
		yStreamIn2 >> y;
		polStreamIn2 >> pol;
		tsStreamIn2 >> ts;
	}

	xStreamOut << x;
	yStreamOut << y;
	polStreamOut << pol;
	tsStreamOut << ts;
}
