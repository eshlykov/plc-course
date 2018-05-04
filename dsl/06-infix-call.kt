infix fun <T> T.shouldBeEqual(expected: T) = this?.equals(expected)

fun main(args: Array<String>) {
    val line = "Hello"
    println(line shouldBeEqual "Hello")
    println(line shouldBeEqual "World")
}
