
PY=python3
VENV=.venv
ACTIVATE=$(VENV)/bin/activate
REQ=requirements.txt

setup: $(VENV)/bin/pip
	. $(ACTIVATE) && pip install -r $(REQ)

$(VENV)/bin/pip:
	python3 -m venv $(VENV)

idle:
	. $(ACTIVATE) && $(PY) experiments/run_idle.py --duration ${DURATION:-28800}

video:
	. $(ACTIVATE) && $(PY) experiments/run_video.py --duration ${DURATION:-3600}

sense:
	. $(ACTIVATE) && $(PY) experiments/run_sensing.py --duration ${DURATION:-3600}

ai:
	. $(ACTIVATE) && $(PY) experiments/run_ai.py --duration ${DURATION:-3600}

game:
	. $(ACTIVATE) && $(PY) experiments/run_game.py --duration ${DURATION:-1800}

arvr:
	. $(ACTIVATE) && $(PY) experiments/run_arvr.py --duration ${DURATION:-1800}

aggregate:
	. $(ACTIVATE) && $(PY) analysis/aggregate.py

tables:
	. $(ACTIVATE) && $(PY) analysis/make_tables.py

analyze: aggregate tables

