#pragma once

#include "../node.h"

#include <vamp-hostsdk/PluginHostAdapter.h>
#include <vamp-hostsdk/PluginInputDomainAdapter.h>
#include <vamp-hostsdk/PluginLoader.h>

#include <algorithm>

using namespace std;

using Vamp::Plugin;
using Vamp::PluginHostAdapter;
using Vamp::RealTime;
using Vamp::HostExt::PluginLoader;
using Vamp::HostExt::PluginWrapper;
using Vamp::HostExt::PluginInputDomainAdapter;

namespace libsignal
{
	class VampAnalysis : public UnaryOpNode
	{
		public:
			VampAnalysis(NodeRef input = 0.0, string plugin_id = "vamp-example-plugins:spectralcentroid:linearcentroid") :
				UnaryOpNode(input)
			{
				this->name = "vamp";

				this->current_frame = 0;

				size_t num_colons = std::count(plugin_id.begin(), plugin_id.end(), ':');
				if (num_colons != 2)
					throw std::runtime_error("Invalid Vamp plugin ID: " + plugin_id);

				int first_separator = plugin_id.find(':');
				int second_separator = plugin_id.find(':', first_separator + 1);
				string vamp_plugin_library = plugin_id.substr(0, first_separator);
				string vamp_plugin_feature = plugin_id.substr(first_separator + 1, second_separator - first_separator - 1);
				string vamp_plugin_output = plugin_id.substr(second_separator + 1);
				signal_debug("Loading plugin %s, %s, %s", vamp_plugin_library.c_str(), vamp_plugin_feature.c_str(), vamp_plugin_output.c_str());

				PluginLoader *loader = PluginLoader::getInstance();
				PluginLoader::PluginKey key = loader->composePluginKey(vamp_plugin_library, vamp_plugin_feature);

				this->plugin = loader->loadPlugin(key, 44100, PluginLoader::ADAPT_ALL);

				if (!this->plugin)
					throw std::runtime_error("Failed to load Vamp plugin: " + plugin_id);

				/*------------------------------------------------------------------------
				 * Get required output index.
				 *-----------------------------------------------------------------------*/
				Plugin::OutputList outputs = this->plugin->getOutputDescriptors();
				this->output_index = -1;

				for (int oi = 0; oi < outputs.size(); oi++)
				{
					if (outputs[oi].identifier == vamp_plugin_output)
					{
						this->output_index = oi;
						break;
					}
				}

				signal_debug("Loaded plugin (output index %d)", this->output_index);
				this->plugin->initialise(1, 512, 512);
			}

			~VampAnalysis()
			{
				// close vamp plugins
			}

			void next(sample **out, int num_frames)
			{
				RealTime rt = RealTime::frame2RealTime(this->current_frame, 44100);
				Plugin::FeatureSet features = this->plugin->process(this->input->out, rt);
				if (features[this->output_index].size())
				{
					Plugin::Feature feature = features[this->output_index][0];
					float value = feature.values[0];
					// printf("Got results (size = %d): %f\n", features[this->output_index].size(), value);

					for (int frame = 0; frame < num_frames; frame++)
					{
						for (int channel = 0; channel < this->channels_in; channel++)
						{
							out[channel][frame] = value;
						}
					}

					this->current_frame += num_frames;
				}
			}

			int current_frame;
			int output_index;
			Plugin *plugin;
	};

	REGISTER(VampAnalysis, "vamp");
}