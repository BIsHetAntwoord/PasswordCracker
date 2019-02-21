Dictionary = {}

function Dictionary:new(native_dict)
    local new_dict = {}
	setmetatable(new_dict, self)
	self.__index = self

	new_dict.native_dict = native_dict

	return new_dict
end

function Dictionary:addName(name)
	Program.Dictionary.addName(self.native_dict, name)
end

function Dictionary:addDate(day, month, year)
	Program.Dictionary.addDate(self.native_dict, day, month, year)
end

function Dictionary:addWord(...)
	Program.Dictionary.addWord(self.native_dict, ...)
end

return Dictionary
