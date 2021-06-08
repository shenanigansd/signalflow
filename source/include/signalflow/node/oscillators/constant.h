#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Constant : public Node
{
public:
    Constant(sample value = 0);

    float value;
    virtual float get_value() override;

    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(Constant, "constant")
}
