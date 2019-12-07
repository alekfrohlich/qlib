.PHONY: format

format:
	find .. -regex '.*\.\(cc\|c\|h\)' -exec clang-format-8 style=.clang-format -i {}   \;

	#2> /dev/null