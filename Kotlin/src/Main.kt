const val NUM_GENES = 12
const val SIZE_GENE = 16
const val CROSSOVER_CHANCE = 0.7
const val MUTATION_CHANCE = 0.01
const val TOTAL_NUM_CHROMOSOMES = 1000

fun rouletteSelect(chrFitPairs: List<Pair<Chromosome, Double>>, totalFitness: Double): Chromosome {
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
  println("Target: " + target)

  println("Forming Generation 0...")
  var currentGen = Array(TOTAL_NUM_CHROMOSOMES, {Chromosome()})
  var solution = currentGen.dropWhile({c -> c.compareTo(target) != 0}).getOrNull(0)
  var generation = 0
  while (solution == null) {
    println("Random: " + currentGen[50])
    val currentGenFitness = currentGen.map{c -> c.fitness(target)}
    val totalFitness = currentGenFitness.reduce(Double::plus)
    val chrFitPairs = currentGen.zip(currentGenFitness)

    val childPairs: Array<Pair<Chromosome, Chromosome>> = Array(TOTAL_NUM_CHROMOSOMES / 2, {
      rouletteSelect(chrFitPairs, totalFitness).breed(
          rouletteSelect(chrFitPairs, totalFitness))
    })

    println("Generation " + generation + " have been bred...")
    currentGen = Array(TOTAL_NUM_CHROMOSOMES, {i ->
      if (i % 2 == 0) childPairs[i / 2].first else childPairs[i / 2].second
    })
    solution = currentGen.dropWhile({c -> c.compareTo(target) != 0}).getOrNull(0)
    ++generation
  }

  println("Solution found: " + solution)
}

