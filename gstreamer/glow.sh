#!/usr/bin/bash

gst-launch-1.0 \
	videotestsrc ! video/x-raw, width=800, height=600 ! tee name=src \
	videomixer name=mix \
			sink_0::zorder=0 \
			sink_1::zorder=1 sink_1::alpha=0.5 \
			sink_2::zorder=2 sink_1::alpha=0.5 \
		! autovideoconvert ! xvimagesink \
	src. ! queue ! mix.sink_0 \
	src. ! queue ! autovideoconvert ! videoscale \
		! video/x-raw, width=8, height=8 ! videoscale \
		! video/x-raw, width=800, height=600 ! mix.sink_1

