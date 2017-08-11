import kotlin.experimental.and
import kotlin.experimental.inv

class Gene(val value: Short) {
  fun createMask(bits: Int): Short {
    var mask = 0
    for (i in 0..bits) {
      mask = mask.shl(1)
      ++mask
    }
    return mask.toShort()
  }

  fun crossover(other: Gene, index: Int): Gene {
    val mask = createMask(index)
    val result: Short = other.value.and(mask.inv()).plus(value.and(mask)).toShort()
    return Gene(result);
  }
}