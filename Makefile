install:
	brew install clang-format doxygen
	sudo pip install cpplint

show:
	tests/lint_all.py --show

reformat:
	tests/lint_all.py --reformat

lint:
	tests/lint_all.py --lint

docs:
	doxygen doxygen.cfg

.PHONY: docs
