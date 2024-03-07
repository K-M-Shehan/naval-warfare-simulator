# naval-warfare-simulator

*insert gif of finished project*

## Overview
1 Battleship and several escorts.

4 different battleships
5 different escort ships

Battleships:
| Name                 | Notation | Shell Velocity |
|----------------------|----------|----------------|
| USS Iowa (BB-61)     | U        | v (rand)       |
| MS King George V     | M        | v (rand)       |
| Richelieu            | R        | v (rand)       |
| Sovetsky Soyuz-class | S        | v (rand)       |

Escort ships:
| Name                    | Notation      | Angle Range | Impact Power | Shell Velocity |
|-------------------------|---------------|-------------|--------------|----------------|
| 1936A class Destroyer   | E<sub>A</sub> | 20          | 0.08         | v * 1.2        |
| Gabbiano class Corvette | E<sub>B</sub> | 30          | 0.06         | e (rand)       |
| Matsu class Destroyer   | E<sub>C</sub> | 25          | 0.07         | e (rand)       |
| F class Escort ships    | E<sub>D</sub> | 50          | 0.05         | e (rand)       |
| Japanese Kaibokan       | E<sub>E</sub> | 70          | 0.04         | e (rand)       |

**NOTE:** 
* v = max velocity of a battleship shell
* Angle range = 90 - rand(angle)
* All e value must be lower than v

Battleship belongs to the Allies.
Escort ships belong to the Axis powers.

Each escort ship represents both the escort ship and the passenger ship. (passenger ships are protected by escort ships)

The battleship has 1 gun.
The escort ships also have 1 gun per ship.

A shell will follow typical parabolic projectile motion under the effect of gravity.
If a shell hits a ship it should explode.
This is an impact.

Air resistance is thrown out of the window as usual.

## Objective
The Battleship should destroy as many escorts as possible.
The Battleship must try to minimize the impact on themselves from the attacks from escort ships.

## Features
1. Horizontal angle: guns in both types of ships can move in any direction within the plane
2. Vertical angle: 
    * battleship - any angle
    * escort ship - differs for each type of ship
3. Initial speed of shell: differs for each ship
4. Max speed of battleship shell: user-specified
5. Min speed of battleship shell: 0
6. 2D canvas: should be a square
7. Number of escort ships: decided with user input
8. Positions of escort ships: randomly generated
9. Type deployed: randomly selected from the database
10. Randomly generated numbers: maximum must be decided beforehand
11. Battleship starting position: given by the user or decided beforehand
12. Ship motion: there will be no motion so ship velocities are equal to 0
