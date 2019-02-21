return function(password)
	print(password)
	if password == "1234567890" then
		return Program.Password.FOUND
	end
	return Program.Password.NOT_FOUND
end