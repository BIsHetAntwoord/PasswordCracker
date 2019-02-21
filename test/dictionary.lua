DictionaryParser = require 'std.dictionaryparser'

return function(dict)
	dict:addName("Marcel Huijben")
	
	local parser = DictionaryParser.new(dict)
	assert(parser:parse(Program.resolvePath("std:common-en.dict")))
end