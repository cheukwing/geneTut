class Chromosome private constructor(val genes: Array<Gene>) {

  constructor() : this(
      Array(NUM_GENES, {
        Gene((Math.random() * ('z'.toInt() + 1 - ' '.toInt()) + ' '.toInt()).toShort())
      }))

  constructor(str: String) : this(str.map { Gene(it) }.toTypedArray())

  private fun mutate(): Chromosome {
    return Chromosome(genes.map({ it.mutate() }).toTypedArray())
  }

  private fun copyOf(): Chromosome {
    return Chromosome(genes.copyOf())
  }

  fun breed(partner: Chromosome): Pair<Chromosome, Chromosome> {
    var fstChild = this.copyOf()
    var sndChild = partner.copyOf()
    if (Math.random() < CROSSOVER_CHANCE) {
      val crossoverPos = (Math.random() * NUM_GENES * SIZE_GENE).toInt()
      val geneIndex = crossoverPos / SIZE_GENE
      val innerIndex = crossoverPos % SIZE_GENE
      val fstMix = this.genes[geneIndex]
          .crossover(partner.genes[geneIndex], innerIndex)
      val sndMix = partner.genes[geneIndex]
          .crossover(partner.genes[geneIndex], innerIndex)

      val fstGenes = Array(NUM_GENES, {
        if (it < geneIndex) {
          this.genes[it]
        } else if (it == geneIndex) {
          fstMix
        } else {
          partner.genes[it]
        }
      })

      val sndGenes = Array(NUM_GENES, {
        if (it < geneIndex) {
          partner.genes[it]
        } else if (it == geneIndex) {
          sndMix
        } else {
          this.genes[it]
        }
      })

      fstChild = Chromosome(fstGenes)
      sndChild = Chromosome(sndGenes)
    }
    fstChild = fstChild.mutate()
    sndChild = sndChild.mutate()

    return Pair(fstChild, sndChild)
  }

  fun fitness(target: Chromosome): Double {
    val sumDiff = genes.zip(target.genes)
        .map { (i, j) -> i.absDiff(j) }
        .sum()
    return 1.0 / sumDiff
  }

  override fun toString(): String {
    return genes.map { it.value.toChar() }.toString()
  }

  fun compareTo(other: Chromosome): Int {
    val thisGenes = this.genes
    val size = thisGenes.size
    val otherGenes = other.genes
    if (otherGenes.size == size) {
      for (i in 0..size - 1) {
        val diff = thisGenes[i].value - otherGenes[i].value
        if (diff != 0) {
          return diff
        }
      }
      return 0
    }
    return size - otherGenes.size
  }
}
