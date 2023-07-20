# ImHexQb

An experimental ImHex plugin for partial qb script recompilation

Please, keep in mind that not every instruction is supported yet!

<p align="center">
    <img src="https://media.giphy.com/media/pJ68r3Gd2mVt2RswGV/giphy.gif" width="480" height="270" alt="ImHexQb Demo">
</p>

## Feedback

If you have any suggestions, contact me on discord: [@hardronix](https://discordapp.com/users/450752497523621908)
## Building

Building a plugin works similarly to building ImHex. Make sure you have all dependencies installed that are necessary to build ImHex itself. Afterwards simply use cmake in the top level of this repository to build libimhex and the plugin. Consult the ImHex README.md for a more in-depth guide on how to compile ImHex using cmake.

## FAQ

#### What's so special about it?

In the realm of thug 1, there exist certain scripts,
that resist decompilation, causing frustrating rifts.
But fear not, for ImHexQb arrives with its light,
shining brightly, illuminating even the darkest night!

ImHexQb possesses a unique talent and skill,
partial decompilation is its power to fulfill.
It takes those stubborn script instructions in its hold,
and translates them into syntax, human-friendly and bold

Once the understanding is achieved and complete,
ImHexQb does more to make the process neat.
With its magic touch, it compiles the code back,
restoring the instruction set in the same place, on track!~

#### What titles will it support?

Well, while it works mostly with Tony Hawk's Underground 1 and Tony Hawk's Underground 2, support for all classic THPS titles will be added later :3
## List Of Supported Instructions

#### De-compilation:
Almost everything, except for:

+ Random
+ Random range
+ Switch cases
+ Goto
+ At

#### Re-compilation:

+ New Instruction
+ Equals (assignment)
+ Equals (comparison)
+ Less Than
+ Less Than Or Equals
+ More Than
+ More Than Or Equals
+ Struct / Struct End
+ Array / Array End
+ Long Integer
+ Float Integer
+ String
+ Checksum
+ Dot
+ Comma
+ Minus
+ Plus
+ Multiply
+ Divide