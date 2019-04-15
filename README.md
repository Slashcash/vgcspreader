# VGCSpreader
VGCSpreader is an algorithm to create efficient spreads for Pokemon in the VGC format. It currently is in a testing phase and has limited capabilities. As of now only some items, moves and abilities are supported.

## How do i use it?
Simply: choose the Pokémon you want to calculate the spread for by completing the form in the **Pokemon** section. You can select its ability, item and nature and you can specify how many evs you already assigned to other stats. The algorithm will then assign the remaining evs in the best possible way given some benchmarks you decide to hit.

You can now move on to define which attacks you want to survive with your pokemon and which ko you need to get. Since VGC is a double format and you can be targeted by two attacks in a turn instead of one, VGCSpreader lets you define a **turn** you need to survive. A turn consisting of a couple of attacks or a single attack in perfect VGC fashion.

**EXAMPLE:** I want an Adamant Incineroar that can resist a combination of a Fake Out by an opposing Incineroar + a Precipice Blades from an opposing Primal Groudon, both at -1 attack. I also want my Incineroar to survive two consecutive Kartana's Sacred Sword at -1. On top of that i want my Incineroar to be able to 1HKO an opposing Lunala with a -1 Malicious Moonsault. VGCSpreader can find the spread that does this in a matter of seconds.

![Example](https://i.ibb.co/Q9fLSBH/vgc-1.png) ![Example](https://i.ibb.co/pwxkFYM/vgc-2.png) ![Example](https://i.ibb.co/BfmXw5Z/vgc-3.png)

Be aware that:
1) You can add as many turn as you want, VGCSpreader will calculate the spread that guarantees you to hit all the benchmarks.

2) VGCSpreader provides the **minimum** number of evs in order to hit all the benchmarks you inserted. You are free to distribute the remaining evs where you need them most.

3) When calculating for a defensive benchmark the order in which you insert the attacks in a turn **does matter**, calculating a spread for a Knock Off + Water Spout on an AV Yveltal is different than calculating a Water Spout + Knock Off (as Water Spout will deal more damage in the latter case than in the former due to the effect of Knock Off of removing items)

After having inserted all the benchmarks you need to hit, you can then click the **Calculate** button and a prompt with the calculated evs distribution will pop-up alongside a useful damage calculation for the turns you inserted.

## What happens if there is no valid evs distribution to hit all the inserted benchmarks?
VGCSpreader will take care of finding a distribution that will guarantee to hit all the benchmarks with varying degree of chance.

**EXAMPLE:** Let's say you want a Careful Tsareena with 244 evs already assigned in speed that can take a Water Spout from Kyogre as a 3HKO and can also survive a Flare Blitz from Incineroar. Both of the benchmarks can only be hit with a roll, but there is a distribution that guarantees a better roll on Water Spout and another one that guarantees a better roll on Flare Blitz. VGCSpreader will output the evs distribution that guarantees better roll for attack you inserted **first**. As you can see from the screenshots inserting the turn in a different order provides two different distributions: the first one giving you a better chance to survive two Water Spouts and the second one a better chance to survive a Flare Blitz.
The same mechanism can be applied to offensive benchmarks.

![Example](https://i.ibb.co/YyMpbx9/vgc-4.png) ![Example](https://i.ibb.co/PYMz5st/vgc-5.png)

Long story short: benchmarks you want to hit have to be inserted in an order of importance.

## What's with the Prioritize Defense / Prioritize Attack option?
If you have some defensive and offensive benchmark that can only be hit with rolls at best, prioritizing the offensive or the defensive side will output spreads that give better rolls on the defensive or offensive side.

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
Heatproof  
Neuroforce  
Fur Coat  
Scrappy  

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
