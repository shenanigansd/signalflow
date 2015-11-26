#include "granulator.h"

#include "constant.h"

namespace signum::gen
{

Granulator::Granulator(Buffer *buffer, UnitRef clock, UnitRef pos, UnitRef grain_length) :
	buffer(buffer), pos(pos), clock(clock), grain_length(grain_length)
{
	this->add_param("pos", this->pos);
	this->add_param("clock", this->clock);
	this->add_param("grain_length", this->grain_length);

	this->channels_out = 2;

	this->pan = 0.5;
	this->add_param("pan", this->pan);

	this->clock_last = 0.0;
}

void Granulator::set_spatialisation(int num_channels, UnitRef pan)
{
	this->channels_out = num_channels;
	this->pan = pan;
}

void Granulator::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		sample pos = this->pos->out[0][frame];
		sample clock_value = this->clock->out[0][frame];
		sample grain_length = this->grain_length->out[0][frame];
		sample pan = this->pan->out[0][frame];

		if (clock_value > clock_last)
		{
			Grain *grain = new Grain(buffer, pos * buffer->sample_rate, grain_length * buffer->sample_rate, pan);
			this->grains.push_back(grain);
		}
		clock_last = clock_value;

		for (int channel = 0; channel < this->channels_out; channel++)
			out[channel][frame] = 0.0;

		std::vector<Grain *>::iterator it;
		for (it = this->grains.begin(); it < this->grains.end(); )
		{
			Grain *grain = *it;
			if (!grain->finished())
			{
				int buffer_index = (grain->sample_start + grain->samples_done) % this->buffer->num_frames;
				sample s = this->buffer->data[0][(int) buffer_index];

				int half_grain_samples = grain->sample_length / 2;
				float amp;
				if (grain->samples_done <= half_grain_samples)
					amp = (float) grain->samples_done / half_grain_samples;
				else
					amp = 1.0 - (float) (grain->samples_done - half_grain_samples) / half_grain_samples;

				grain->samples_done++;

				for (int channel = 0; channel < this->channels_out; channel++)
					out[channel][frame] += s * amp;

				it++;
			}
			else
			{
				delete grain;
				grains.erase(it);
			}
		}
	}
}


}
