install:
	brew install clang-format doxygen
	sudo pip install cpplint

show:
	scripts/lint_all.py --show

reformat:
	scripts/lint_all.py --reformat

lint:
	scripts/lint_all.py --lint

docs:
	doxygen doxygen.cfg

bundle:
	scripts/bundle.sh

.PHONY: docs
