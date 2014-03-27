
local gst = require 'gst'

elements = gst.list()
for name,prop in pairs(elements) do
	for k,v in pairs(prop) do
		print(("%s.%s = %q"):format(name, k, v))
	end
end

