class Chromosome private constructor(val genes: Array<Gene>) {

  fun generateRandom(): Chromosome {
    val randGenes = Array(NUM_GENES, {i -> Gene((Math.random() * Short.MAX_VALUE).toShort())})
    return Chromosome(randGenes);
  }

  private fun mutate() {
    genes.map({it.mutate()})
  }

  private fun copyOf(): Chromosome {
    return Chromosome(genes.copyOf())
  }

  fun breed(partner: Chromosome): Children {
    var fstChild = this.copyOf()
    var sndChild = partner.copyOf()
    if (Math.random() < CROSSOVER_CHANCE) {
      val crossoverPos = (Math.random() * NUM_GENES * SIZE_GENE).toInt()
      val geneIndex = crossoverPos / NUM_GENES
      val innerIndex = crossoverPos % NUM_GENES
      val fstMix = this.genes[geneIndex].crossover(partner.genes[geneIndex], innerIndex)
      val sndMix = partner.genes[geneIndex].crossover(partner.genes[geneIndex], innerIndex)

      val fstGenes = Array(NUM_GENES, {i ->
        if (i < geneIndex) {
          this.genes[geneIndex]
        } else if (i == geneIndex) {
          fstMix
        } else {
          partner.genes[geneIndex]
        }
      })

      val sndGenes = Array(NUM_GENES, {i ->
        if (i < geneIndex) {
          partner.genes[geneIndex]
        } else if (i == geneIndex) {
          sndMix
        } else {
          partner.genes[geneIndex]
        }
      })

      fstChild = Chromosome(fstGenes)
      sndChild = Chromosome(sndGenes)
    }
    fstChild.mutate()
    sndChild.mutate()
    return Children(fstChild, sndChild)
  }

}