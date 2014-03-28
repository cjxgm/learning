
local sleep = function(seconds)
	local start = os.clock()
	while os.clock()-start < seconds do end
end

local gst = require 'gst'

-- print all elements
local elements = gst.elements()
local fmt = "%s\n    %-76.76s\n    %-76.76s\n    %-76.76s\n"
for name,prop in pairs(elements) do
	print(fmt:format(name, prop.category, prop.longname, prop.description))
end

-- create a pipeline: videotestsrc ! xcimagesink
local pipe = gst.element('pipeline')
local src = gst.element('videotestsrc')
local sink = gst.element('xvimagesink')
gst.add(pipe, src)
gst.add(pipe, sink)
gst.link(src, sink)

-- play the pipeline
gst.play(pipe)
sleep(1)
gst.stop(pipe)
sleep(0.5)
gst.play(pipe)
sleep(2)

