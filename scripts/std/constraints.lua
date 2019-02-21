PasswordConstraints = {}

function PasswordConstraints:new(native_constr)
	local new_constr = {}
	setmetatable(new_constr, self)
	self.__index = self
	
	new_constr.native_constr = native_constr
	
	return new_constr
end

function PasswordConstraints:setMinLength(length)
	Program.PasswordConstraints.setMinLength(self.native_constr, length)
end

function PasswordConstraints:setMaxLength(length)
	Program.PasswordConstraints.setMaxLength(self.native_constr, length)
end

function PasswordConstraints:requireOneOf(chars)
	Program.PasswordConstraints.requireOneOf(self.native_constr, chars)
end

return PasswordConstraints