// StreamTransformer.cpp
// author: Johannes Wagner <wagner@hcm-lab.de>
// created: 2013/06/06
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

#include "StreamTransformer.h"
#include "base/Factory.h"

#ifdef USE_SSI_LEAK_DETECTOR
#include "SSI_LeakWatcher.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

namespace ssi {

	StreamTransformer::StreamTransformer(const ssi_char_t *file) : _file(0), _lastValueX(0), _lastValueY(0) {

		if (file) {
            if (!OptionList::LoadXML(file, &_options)) {
                OptionList::SaveXML(file, &_options);
			}
			_file = ssi_strcpy(file);
		}
	}

	StreamTransformer::~StreamTransformer() {

		if (_file) {
            OptionList::SaveXML(_file, &_options);
			delete[] _file;
		}
	}

	void StreamTransformer::transform_enter(ssi_stream_t &stream_in,
		ssi_stream_t &stream_out,
		ssi_size_t xtra_stream_in_num,
		ssi_stream_t xtra_stream_in[]) {
	}

	void StreamTransformer::transform(ITransformer::info info,
		ssi_stream_t &stream_in,
		ssi_stream_t &stream_out,
		ssi_size_t xtra_stream_in_num,
		ssi_stream_t xtra_stream_in[]) {

		ssi_size_t dim = stream_in.dim;
		ssi_size_t num = info.frame_num;

		ssi_real_t *srcptr = ssi_pcast(ssi_real_t, stream_in.ptr);
		ssi_real_t *dstptr = ssi_pcast(ssi_real_t, stream_out.ptr);

		transformThreshold(dim, num, srcptr, dstptr);
	}

	void StreamTransformer::transformThreshold(ssi_size_t dim, ssi_size_t num, ssi_real_t *srcptr, ssi_real_t *dstptr)
	{
		for (ssi_size_t i = 0; i < num * dim; i++)
		{
			ssi_real_t value = *srcptr++;

			if (i > 0)
			{
				if (i == 1)
				{
					if (_options.absolute)
					{
						if (value > _options.upperThreshX || value < _options.lowerThreshX)
						{
							value = _lastValueX;
						}
					}
					else
					{
						if (value - _lastValueX > _options.maxDiff)
						{
							value = _lastValueX + _options.maxDiff;
						}
						else if (value - _lastValueX < -1 * _options.maxDiff)
						{
							value = _lastValueX - _options.maxDiff;
						}
					}

					_lastValueX = value;
				}
				else if (i == 2)
				{
					if (_options.absolute)
					{
						if (value > _options.upperThreshY || value < _options.lowerThreshY)
						{
							value = _lastValueY;
						}
					}
					else
					{
						if (value - _lastValueY > _options.maxDiff)
						{
							value = _lastValueY + _options.maxDiff;
						}
						else if (value - _lastValueY < -1 * _options.maxDiff)
						{
							value = _lastValueY - _options.maxDiff;
						}
					}

					_lastValueY = value;
				}
			}

			*dstptr++ = value;
		}
	}

	void StreamTransformer::transform_flush(ssi_stream_t &stream_in,
		ssi_stream_t &stream_out,
		ssi_size_t xtra_stream_in_num,
		ssi_stream_t xtra_stream_in[]) {
	}


}
