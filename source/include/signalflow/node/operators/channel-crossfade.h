#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Given a multichannel input, crossfades between channels based on the given
 * position within the virtual array, producing a single-channel output.
 *---------------------------------------------------------------------------------*/
class ChannelCrossfade : public UnaryOpNode
{

public:
    ChannelCrossfade(NodeRef input = nullptr,
                     NodeRef index = nullptr,
                     int num_output_channels = 1);

    virtual void process(Buffer &out, int num_frames);

private:
    NodeRef index;
    int num_output_channels;
};

REGISTER(ChannelCrossfade, "channel-crossfade")

}
