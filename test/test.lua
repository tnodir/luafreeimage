#!/usr/bin/env lua

local fi = require"freeimage"

local fiu = fi.util


print(fi.getInfo())

local dib = fi.bitmap()

assert(dib:load("gif", "fig3.gif"))

assert(dib:getBackgroundColor())

print("colorsUsed", dib:getColorsUsed())


print"-- Make Thumbnail"
do
	local thumb = fi.bitmap()
	assert(thumb:makeThumbnail(dib, 128))
	assert(thumb:save("jpeg", "thumb.jpg"))
	os.remove("thumb.jpg")
	print("OK")
end

