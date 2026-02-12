# Week 4 — Sprites + Collisions

Simple game exploring sprites, collisions, collision rules, and collision feedback.

## Run
From this folder:

- `python3 -m pip install -r requirements.txt`
- `python3 main.py`

## Controls
- Arrow keys / WASD: move
- `F1`: toggle debug (hitboxes)
- `R`: reset
- `Esc`: quit

## This Week
- Wave progression with increasing difficulty
- Power-up: invulnerability
- Animation feedback for coin pick-up

## Core Game Loop

Player moves around the playfield, trying to collect coins.  When all the coins on screen are collected, the wave is complete.  Each wave spawns coins randomly and increases speed of obstacles.  