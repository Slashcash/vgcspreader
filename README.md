# VGCSpreader
VGCSpreader is an algorithm to create efficient spreads for Pokemon in the VGC format. It currently is in a testing phase and has limited capabilities. As of now only some items, moves and abilities are supported.

## How do i use it?
As of now VGCSpreader can only calculate defensive spread, however the tool is constructed to calculate the spread for both the offensive and defensive side and working on this feature is the immediate plan.

Simply: choose the Pokémon you want to calculate the defensive spread for by completing the form in the **Defending Pokemon** section. You can select its ability, item and nature and you can specify how many evs you already assigned to other stats. The algorithm will then assign the remaining evs to defenses in the best way possible.

You can now move on to define whichever attacks you want to survive. In VGCSpreader attacks are not just attacks, you can define a **turn** you need to survive and you can do so by clicking the add button.

**EXAMPLE:** I want an AV Yveltal that can survive a turn in which the opponents throws off a Water Spout with a Kyogre and an Hurricane with a Tornadus. I have to click **Add**, then adding the specified move with the correct parameters. 

Be aware that:
1) You can add as many turn as you want, VGCSpreader will calculate the spread that will make the defending pokemon survive all the turns.

2) VGCSpreader provides the **minimum** number of evs you need to survive all the turns you inserted. You are free to distribute the remaining evs where you need them most.

2) A turn can of course be formed by just a single attack instead of two, you just need to de-activate the **Attacking Pokemon No.2** section by clicking the obvious checkbox

3) The order of a turn **does matter**, calculating a spread for a Knock Off + Water Spout on an AV Yveltal is different than calculating a Water Spout + Knock Off (as water spout will deal more damage in the latter case than in the former due to the effect of Knock Off of removing items)

After having inserted all the turns you wish to resist, you can then click the **Calculate** button and a prompt with the calculated evs distribution will pop-up alongside a useful damage calculation for the turns you inserted.

## What happens if there is no valid evs distribution to survive the inserted turns?
VGCSpreader will take care of finding a distribution that will guarantee the survivability of the defending Pokémon with varying degree of chance.

**EXAMPLE:** While there is no valid spread for a Careful Incineroar to survive a Fake out and a +2 Xerneas Moonblast in the same turn 100% of the time VGCSpreader can find a spread that guarantees surviving in most of the cases.

**EXAMPLE 2:** Let's say you want a Careful Tsareena with 244 evs already assigned in speed that can take a Water Spout from Kyogre as a 3HKO and can survive a Water Spout from Kyogre + an Ice Beam from Ludicolo. Both of the benchmarks can only be hit with a roll, but there is a distribution that guarantees a better roll on Water Spout and another one that guarantees a better roll on Water Spout + Ice Beam. VGCSpreader will output the evs distribution that guarantees better roll for attack you inserted **first**. In this case the evs distribution provided will guarantee a better chance to survive Water Spout with a 3HKO but inverting the insertion order will guarantee a better survivability on Water Spout + Ice Beam.
Long story short: attacks you need to survive have to be inserted in order of importance.

## Item, Moves and abilities supported 
As this is still in a testing phase only a limited amount of item, moves and abilities are supported with a correct damage calculation:

**ABILITIES**  
Blaze  
Fairy Aura  
Shadow Shield  
Dark Aura  
Overgrow  
Adaptability  
Prism Armor  
Wonder Guard  
Cloud Nine  
Air Lock  
Multiscale  
Filter  
Solid Rock  
Turboblaze  
Teravolt  
Levitate  
Aerilate  
Pixilate  
Refrigerate  
Normalize  
Galvanize  
Huge Power  
Parental Bond  

Abilities that do not affect damage caclculation like Intimidate are normally supported.

**ITEM**  
Life Orb  
Assault Vest  
Choice Band  
Choice Specs  
All the damage reduction berries (es: Payapa Berry)  

Items which do not affect damage calculation like Eject Button are normally supported.

All the damaging moves are supported, moves with secondary effects are generally not supported with the exception of:

**MOVES**  
Acrobatics  
Darkest Lariat  
Facade  
Brine  
Water Spout  
Sacred Sword  
Psyshock  
Foul Play  
Eruption  
Knock Off  
Moongeist Beam  
Menacing Moonraze Maelstrom  
Sunsteel Strike  
Searing Sunraze Smash  

## Please be aware
This is still a software in an heavy beta state and under testing, always make sure the distribution it calculates are correct using an external damage calculator.

## Thanks to:
**kwsch** for PKHex @ https://github.com/kwsch/PKHeX  
Some resources are taken from there

**Honko** for the Showdown Damage Calculator @ https://github.com/Zarel/honko-damagecalc  
the damage calculation process is taken from there

**Flavio Del Pidio** italian VGC player that helped me in the testing phase

The Survival Calculator on Trainer Tower which i took the idea from

## Contributing
Feel free to take the code, branch and contribute. You can contact me at: gen.iorio92@gmail.com or here on github.
