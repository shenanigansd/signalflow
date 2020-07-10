#include "signalflow/node/processors/waveshaper.h"

namespace signalflow
{

WaveShaper::WaveShaper(NodeRef input, BufferRef buffer)
    : UnaryOpNode(input), buffer(buffer)
{
    this->name = "waveshaper";
}

void WaveShaper::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float input = this->input->out[channel][frame];
            out[channel][frame] = this->buffer->get(input);
        }
    }
}

}