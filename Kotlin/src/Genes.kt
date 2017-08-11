class Genes(var contents: Array<Gene>) {
  fun copyOf(): Genes {
    val arrayCopy: Array<Gene> = this.contents.copyOf();
    return Genes(arrayCopy);
  }
}
