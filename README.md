# naval-warfare-simulator

## Overview
1 Battleship and several escorts.

4 different battleships
5 different escort ships

Battleship belongs to the allies.
Escort ships belong to the axis powers.

Each escort ship represents both the escort ship and passenger ship. (passenger ships are protected by escort ships)

The battleship has 1 gun.
The escort ships also have 1 gun per ship.

A shell will follow typical parabloic projectile motion under the effect of gravity.
If shell hits a ship it should explode.
This is an impact.

Air resistance is thrown out of the window as usual.

## Objective
Battleship should destroy as many escorts as possible.
Battleship must try to minimize impact on itself from the attacks from escort ships.

## Features
1. Horizontal angle: guns in both types of ships can move in any direction within plane
2. Vertical angle: 
  * battleship - any angle
  * escort ship - differs for each type of ship
3. Initial speed of shell: differs for each ship
4. Max speed of battleship shell: user specified
5. Min speed of battleship shell: 0
6. 2D canvas: should be a square
7. Number of escort ships: decided with user input
8. Positions of escort ships: randomly generated
9. Type deployed: randomly selected from database
10. Randomly generated numbers: maximum must be decided beforehand
11. Battleship starting position: given by user or decided beforehand
12. Ship motion: there will be no motion so ship velocities are equal to 0
