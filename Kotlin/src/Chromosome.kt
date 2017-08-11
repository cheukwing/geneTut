class Chromosome private constructor(val genes: Array<Gene>) {

  constructor(): this(Array(NUM_GENES, {Gene((Math.random() * Short.MAX_VALUE).toShort())}))

  constructor(str: String): this(str.map{c -> Gene(c)}.toTypedArray())

  private fun mutate(): Chromosome {
    return Chromosome(genes.map({it.mutate()}).toTypedArray())
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
      val fstMix = this.genes[geneIndex].crossover(partner.genes[geneIndex], innerIndex)
      val sndMix = partner.genes[geneIndex].crossover(partner.genes[geneIndex], innerIndex)

      val fstGenes = Array(NUM_GENES, {i ->
        if (i < geneIndex) {
          this.genes[i]
        } else if (i == geneIndex) {
          fstMix
        } else {
          partner.genes[i]
        }
      })

      val sndGenes = Array(NUM_GENES, {i ->
        if (i < geneIndex) {
          partner.genes[i]
        } else if (i == geneIndex) {
          sndMix
        } else {
          this.genes[i]
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
    val sumDiff = genes.zip(target.genes).map{(i, j) -> i.absDiff(j)}.reduce(Int::plus)
    return 1.0 / sumDiff
  }

  override fun toString(): String {
    return genes.map{g -> g.value.toChar()}.toString()
  }

  fun compareTo(other: Chromosome): Int {
    val thisGenes = this.genes
    val size = thisGenes.size
    val otherGenes = other.genes
    if (otherGenes.size == size) {
      for (i in 0..size) {
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