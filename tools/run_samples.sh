#!/usr/bin/env bash
# Run every solution on the sample tests that ship with this repo.
set -e
cd "$(dirname "$0")/.."
python3 tools/check_official.py bronze1 bronze/1-mooin-time-iv/samples
python3 tools/check_official.py bronze2 bronze/2-moo-hunt/samples
python3 tools/check_official.py bronze3 bronze/3-purchasing-milk/samples
python3 tools/check_official.py silver1 silver/1-cow-libi-2/samples
python3 tools/check_official.py silver2 silver/2-declining-invitations/samples
python3 tools/check_official.py silver3 silver/3-fj-loves-rotations/samples
python3 tools/check_official.py gold1 gold/1-balancing-the-barns/samples
python3 tools/check_official.py gold2 gold/2-lex-smallest-path/samples
python3 tools/check_official.py gold3 gold/3-the-chase/samples
