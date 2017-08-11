const val NUM_GENES = 12
const val SIZE_GENE = 16
const val CROSSOVER_CHANCE = 0.7
const val MUTATION_CHANCE = 0.001

fun rouletteSelect(chrFitPairs: List<Pair<Chromosome, Double>>, totalFitness: Double): Chromosome {
  val selection = Math.random() * totalFitness
  var currTotal = 0.0
  for ((chr, fit) in chrFitPairs) {
    currTotal += fit
    if (selection < currTotal) {
      return chr
    }
  }
  return chrFitPairs.last().first
}


