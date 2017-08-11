# geneTut
Genetic algorithm based on a tutorial from ai-junkie, which can be found at http://www.ai-junkie.com/ga/intro/gat1.html.

## C
The C folder contains a somewhat faithful reproduction of the algorithm from the tutorial.
Changes include the removal of the division operator to remove the need for fiddling with floating point numbers and a higher mutation chance to stop stagnation. I believe this stagnation may be the result of an incorrect implementation but I am unable to find the source.

## Kotlin
The Kotlin folder contains an alternative implementation of the algorithm, whereby the generations must now aim to reproduce a string.
This was made mostly to practise genetic algorithms and try to learn the Kotlin language.
