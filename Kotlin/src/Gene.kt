import kotlin.experimental.and
import kotlin.experimental.inv
import kotlin.experimental.xor

class Gene(val value: Short) {

  constructor(char: Char): this(char.toShort())

  fun createMask(lower: Int, upper: Int): Short {
    var mask = 0
    val range = upper - lower
    for (i in 0..range - 1) {
      mask = mask.shl(1)
      ++mask
    }

    mask.shl(lower)
    return mask.toShort()
  }

  fun crossover(other: Gene, index: Int): Gene {
    val mask = createMask(0, index)
    val result: Short = other.value.and(mask.inv()).plus(value.and(mask)).toShort()
    return Gene(result)
  }

  fun mutate(): Gene {
    var mutationMask = 0
    for (i in 0..SIZE_GENE - 1) {
      if (Math.random() < MUTATION_CHANCE) {
        ++mutationMask
      }
      mutationMask.shl(1);
    }
    return Gene(value.xor(mutationMask.toShort()))
  }

  fun absDiff(other: Gene): Int {
    return Math.abs(this.value - other.value)
  }
}