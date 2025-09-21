.PHONY: setup simulate analyze clean

setup:
	python3 -m venv .venv && . .venv/bin/activate && pip install -r docker/requirements.txt

simulate:
	python scripts/run_experiments.py --seed 42 --reps 30

analyze:
	python analysis/make_tables.py

clean:
	rm -f data/logs/*.csv data/out/*.csv
