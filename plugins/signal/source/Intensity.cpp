// Intensity.cpp
// author: Johannes Wagner <wagner@hcm-lab.de>
// created: 2008/01/18
// Copyright (C) University of Augsburg, Lab for Human Centered Multimedia
//
// *************************************************************************************************
//
// This file is part of Social Signal Interpretation (SSI) developed at the 
// Lab for Human Centered Multimedia of the University of Augsburg
//
// This library is free software; you can redistribute itand/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or any laterversion.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FORA PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along withthis library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
//*************************************************************************************************

#include "Intensity.h"
#include "signal/MatrixOps.h"

#ifdef USE_SSI_LEAK_DETECTOR
	#include "SSI_LeakWatcher.h"
	#ifdef _DEBUG
		#define new DEBUG_NEW
		#undef THIS_FILE
		static char THIS_FILE[] = __FILE__;
	#endif
#endif

namespace ssi {

Intensity::Intensity (const ssi_char_t *file)
	: _sample_number (0),
	_scalar (0),
	_file (0) {

	if (file) {
		if (!OptionList::LoadXML (file, _options)) {
			OptionList::SaveXML (file, _options);
		}
		_file = ssi_strcpy (file);
	}
}

Intensity::~Intensity () {

	if (_file) {
		OptionList::SaveXML (_file, _options);
		delete[] _file;
	}
}

void Intensity::transform (ITransformer::info info,
	ssi_stream_t &stream_in,
	ssi_stream_t &stream_out,
	ssi_size_t xtra_stream_in_num,
	ssi_stream_t xtra_stream_in[]) {

	ssi_size_t sample_dimension = stream_in.dim;
	ssi_size_t sample_number = stream_in.num;

	ssi_real_t *srcptr = ssi_pcast (ssi_real_t, stream_in.ptr);
	ssi_real_t *dstptr = ssi_pcast (ssi_real_t, stream_out.ptr);

	if (_sample_number != sample_number) {
		if (_options.global) {
			_scalar = ssi_cast (ssi_real_t, (1.0 / (sample_number * sample_dimension)) / 4.0e-10);
		} else {
			_scalar = ssi_cast (ssi_real_t, (1.0 / sample_number) / 4.0e-10);
		}
		_sample_number = sample_number;
	}

	if (_options.global) {

		ssi_real_t sum = 0;
		ssi_size_t elems = sample_number * sample_dimension;
		for (ssi_size_t i = 0; i < elems; i++) {
			sum += *srcptr * *srcptr;
			srcptr++;
		}
		*dstptr = *dstptr == 0 ? 0 : 10 * log10(_scalar * sum);

	} else {

		ssi_real_t *dstptr_tmp = dstptr;
		for (ssi_size_t i = 0; i < sample_dimension; i++) {
			*dstptr++ = *srcptr * *srcptr;
			srcptr++;
		}
		for (ssi_size_t i = 1; i < sample_number; i++) {
			dstptr = dstptr_tmp;	
			for (ssi_size_t j = 0; j < sample_dimension; j++) {
				*dstptr++ += *srcptr * *srcptr;
				srcptr++;
			}
		}
		dstptr = dstptr_tmp;
		for (ssi_size_t i = 0; i < sample_dimension; i++) {
			*dstptr = *dstptr == 0 ? 0 : 10 * log10(_scalar * *dstptr);
			dstptr++;
		}
	}
}

}

