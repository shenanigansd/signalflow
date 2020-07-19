#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

class ChannelArray : public Node
{

public:
    ChannelArray();
    ChannelArray(std::initializer_list<NodeRef> inputs);
    ChannelArray(std::vector<NodeRef> inputs);
    ChannelArray(std::vector<int> inputs);
    ChannelArray(std::vector<float> inputs);

    virtual void process(sample **out, int num_frames);
    virtual void update_channels();

    virtual void add_input(NodeRef input);
    virtual void set_input(std::string name, const NodeRef &node);

    std::list<NodeRef> input_list;
};

REGISTER(ChannelArray, "channel-array")

}
