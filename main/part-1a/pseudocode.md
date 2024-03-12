## Part 1 - A

* The gun on the **battleship** can reload and fire in 0 seconds.
* The gun on the **escort ships** can only be fired *once*.
* A *single impact* can destroy a ship, regardless of the type.
* Any ship can fire if the opposing type is in its attack range

![attack range figure](https://imgur.com/ipYkUbS.jpg)

### *Steps* =>
1. Initial conditions must be saved as a text file.
    * Positions
    * max/min velocity of a battleship shell (v)
    * max/min angle
    * type of Escort ship
2. Determine if the battleship is going to sink in this scenario.
3. If B sinks:
    * display the index if the escort ship that sank the battleship
4. If B doesn't sink:
    * display how many escort ships get hit by the battleship 
    * display how long it takes to end the battle
    * save the details of the escort ships that got hit by the battleship
        * index of the escort ship
        * time it takes to hit the escort
        * type of the escort ships that got hit
        * any other values
5. Save the final conditions of the battlefield as a text file.
