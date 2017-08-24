const val NUM_GENES = 12
const val SIZE_GENE = 16
const val CROSSOVER_CHANCE = 0.7
const val MUTATION_CHANCE = 0.001
const val TOTAL_NUM_CHROMOSOMES = 10000

fun rouletteSelect(
    chrFitPairs: List<Pair<Chromosome, Double>>, totalFitness: Double): Chromosome {
  val selection = Math.random() * totalFitness
  var currTotal = 0.0
  for ((chr, fit) in chrFitPairs) {
    currTotal += fit
    if (selection < currTotal) {
      return chr
    }
  }
  println("Probable error occurred...")
  return chrFitPairs.last().first
}


fun main(args: Array<String>) {
  val target = Chromosome("Hello World!")
  println("Target: $target")

  println("Forming Generation 0...")
  var currentGen = Array(TOTAL_NUM_CHROMOSOMES, { Chromosome() })
  var solution = currentGen.dropWhile({ it.compareTo(target) != 0 })
      .firstOrNull()
  var generation = 0

  while (solution == null) {
    println("50th Element:  ${currentGen[50]}")
    val currentGenFitness = currentGen.map { it.fitness(target) }
    val totalFitness = currentGenFitness.sum()
    val chrFitPairs = currentGen.zip(currentGenFitness)

    val childPairs: Array<Pair<Chromosome, Chromosome>> =
        Array(TOTAL_NUM_CHROMOSOMES / 2, {
          rouletteSelect(chrFitPairs, totalFitness)
              .breed(rouletteSelect(chrFitPairs, totalFitness))
        })

    println("Generation $generation have been bred...")

    currentGen = Array(TOTAL_NUM_CHROMOSOMES, {
      if (it % 2 == 0) childPairs[it / 2].first else childPairs[it / 2].second
    })

    solution = currentGen.dropWhile({ it.compareTo(target) != 0 })
        .firstOrNull()

    ++generation
  }

  println("Solution found: $solution")
}

